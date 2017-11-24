#ifndef BOOSTING_MIME_TYPES_HPP
#define BOOSTING_MIME_TYPES_HPP

#include <string>

namespace http {
        namespace mime_types {
            ///Преобразование расщирения файла в MIME тип
            std::string extension_to_type(const std::string& extension);
        }
}

#endif
