/* test driver for newsbeuter */

#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

#include <unistd.h>

#include <cache.h>
#include <rss.h>
#include <configcontainer.h>

#include <stdlib.h>

using namespace newsbeuter;

BOOST_AUTO_TEST_CASE(TestNewsbeuterReload) {
	configcontainer * cfg = new configcontainer();
	cache * rsscache = new cache("test-cache.db", cfg);

	rss_parser parser("http://bereshit.synflood.at/~ak/rss.xml", rsscache, cfg);
	rss_feed feed = parser.parse();
	BOOST_CHECK_EQUAL(feed.items().size(), 8u);

	rsscache->externalize_rssfeed(feed);
	rsscache->internalize_rssfeed(feed);
	BOOST_CHECK_EQUAL(feed.items().size(), 8u);

	BOOST_CHECK_EQUAL(feed.items()[0].title(), "Teh Saxxi");
	BOOST_CHECK_EQUAL(feed.items()[7].title(), "Handy als IR-Detektor");

	feed.items()[0].set_title("Another Title");
	feed.items()[0].set_pubDate(time(NULL));
	BOOST_CHECK_EQUAL(feed.items()[0].title(), "Another Title");

	rsscache->externalize_rssfeed(feed);

	rss_feed feed2(rsscache);
	feed2.set_rssurl("http://bereshit.synflood.at/~ak/rss.xml");
	rsscache->internalize_rssfeed(feed2);

	BOOST_CHECK_EQUAL(feed2.items().size(), 8u);
	BOOST_CHECK_EQUAL(feed2.items()[0].title(), "Another Title");
	BOOST_CHECK_EQUAL(feed2.items()[7].title(), "Handy als IR-Detektor");

	delete rsscache;
	delete cfg;

	::unlink("test-cache.db");
}

BOOST_AUTO_TEST_CASE(TestConfigParserAndContainer) {
	configcontainer * cfg = new configcontainer();
	configparser cfgparser("test-config.txt");
	cfg->register_commands(cfgparser);

	try {
		cfgparser.parse();
	} catch (...) {
		BOOST_CHECK(false);
	}

	// test boolean config values
	BOOST_CHECK_EQUAL(cfg->get_configvalue("show-read-feeds"), "no");
	BOOST_CHECK_EQUAL(cfg->get_configvalue_as_bool("show-read-feeds"), false);

	// test string config values
	BOOST_CHECK_EQUAL(cfg->get_configvalue("browser"), "firefox");

	// test integer config values
	BOOST_CHECK_EQUAL(cfg->get_configvalue("max-items"), "100");
	BOOST_CHECK_EQUAL(cfg->get_configvalue_as_int("max-items"), 100);

	// test ~/ expansion for path config values
	std::string cachefilecomp = ::getenv("HOME");
	cachefilecomp.append("/foo");
	BOOST_CHECK(cfg->get_configvalue("cache-file") == cachefilecomp);

	delete cfg;
}
