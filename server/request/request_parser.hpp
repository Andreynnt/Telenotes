#ifndef BOOSTING_REQUEST_PARSER_HPP
#define BOOSTING_REQUEST_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <boost/tuple/tuple.hpp>

namespace http {

        struct request;

        class request_parser {
        public:

            request_parser();

            /// Сброс в исходное состояние парсера
            void reset();

            ///Парсинг данных, tribool возвращаеи true, когда запрос был полностью распаршен,
            ///false, когда запрос невалидный, indeterminate, когда требуется больше данных
            ///Возвращаемое значение InputIterator указывает, сколько из данных было потреблено.

            template <typename InputIterator>
            boost::tuple<boost::tribool, InputIterator> parse(request& req, InputIterator begin, InputIterator end) {
                while (begin != end) {
                    boost::tribool result = consume(req, *begin++);
                    if (result || !result) {
                        return boost::make_tuple(result, begin);
                    }
                }
                boost::tribool result = boost::indeterminate;
                return boost::make_tuple(result, begin);
            }

        private:
            /// Обработка следующего символа ввода
            boost::tribool consume(request& req, char input);

            /// Проверка, является ли байт символом HTTP.
            static bool is_char(int c);

            /// Проверка, является ли байт символом управления HTTP.
            static bool is_ctl(int c);

            /// Проверка, определен ли байт как HTTP-символ.
            static bool is_tspecial(int c);

            /// Проерка, цифра ли это
            static bool is_digit(int c);

            /// Текущее сосотояние парсера
            enum state {
                method_start,
                method,
                uri,
                http_version_h,
                http_version_t_1,
                http_version_t_2,
                http_version_p,
                http_version_slash,
                http_version_major_start,
                http_version_major,
                http_version_minor_start,
                http_version_minor,
                expecting_newline_1,
                header_line_start,
                header_lws,
                header_name,
                space_before_header_value,
                header_value,
                expecting_newline_2,
                expecting_newline_3
            } state_;
        };

}

#endif //BOOSTING_REQUEST_PARSER_HPP
