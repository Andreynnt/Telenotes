#include "mime_types.hpp"

namespace http {
    namespace mime_types {

        std::map<std::string, std::string> mime_type {
                { "json", "application/json" },
                { "gif", "image/gif" },
                { "htm", "text/html" },
                { "html", "text/html" },
                { "jpg", "image/jpeg" },
                { "png", "image/png" },
        };

        std::string extension_to_type(const std::string& extension) {
           if (mime_type.count(extension) ) {
               return mime_type[extension];
           } else {
               std::string plain("text/plain");
               return plain;
           }
        }
    }
}
