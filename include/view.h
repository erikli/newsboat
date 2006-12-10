#ifndef NOOS_VIEW__H
#define NOOS_VIEW__H

#include <controller.h>
#include <configcontainer.h>
#include <vector>
#include <string>
#include <rss.h>
#include <keymap.h>

extern "C" {
#include <stfl.h>
}

namespace noos {

	class view {
		public:
			view(controller * );
			~view();
			void run_feedlist();
			void run_itemlist(rss_feed& feed);
			bool run_itemview(rss_item& item);
			void run_help();
			void set_feedlist(std::vector<rss_feed>& feeds);
			void set_keymap(keymap * k);
			void set_config_container(configcontainer * cfgcontainer);
			void feedlist_error(const char * msg);
			void itemlist_error(const char * msg);
			void itemview_error(const char * msg);
			void feedlist_status(const char * msg);
			void itemlist_status(const char * msg);
			void itemview_status(const char * msg);
		private:
			bool jump_to_next_unread_item(std::vector<rss_item>& items);
			void mark_all_read(std::vector<rss_item>& items);
			void open_in_browser(const std::string& url);
			
			void set_itemview_keymap_hint();
			void set_itemlist_keymap_hint();
			void set_feedlist_keymap_hint();
			void set_help_keymap_hint();
			
			void set_itemlist_head(const std::string& s, unsigned int unread, unsigned int total);
			void set_itemview_head(const std::string& s);
			
			void write_item(const rss_item& item, const std::string& filename);
			
			enum filebrowser_type { FBT_OPEN, FBT_SAVE };
			
			std::string get_rwx(unsigned short val);
			std::string add_file(std::string filename);
			std::string fancy_quote(const std::string& s);
			std::string fancy_unquote(const std::string& s);
			std::string filebrowser(filebrowser_type type, const std::string& default_filename = "", std::string dir = "");
			std::string get_filename_suggestion(const std::string& s);
			
			struct keymap_hint_entry {
				operation op; 
				char * text;
			};

			std::string prepare_keymaphint(keymap_hint_entry * hints);

			controller * ctrl;
			stfl_form * feedlist_form;
			stfl_form * itemlist_form;
			stfl_form * itemview_form;
			stfl_form * help_form;
			stfl_form * filebrowser_form;
			
			configcontainer * cfg;
			keymap * keys;
	};

}

#endif
