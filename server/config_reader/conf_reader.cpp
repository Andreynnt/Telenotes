#include <boost/property_tree/xml_parser.hpp>
#include "conf_reader.hpp"


databaseConfig database = databaseConfig();


namespace pt = boost::property_tree;

void serverConfig::readConfigFile(const std::string& file) {
    pt::ptree serverTree;
    pt::read_xml(file, serverTree);
    try {
        address = serverTree.get<std::string>("server.address");
        // значение 8000 - по умолчанию
        port = serverTree.get("server.port", 8000);
        // значение 1 - по умолчанию
        threads = serverTree.get("server.threads", 1);

    } catch (boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::property_tree::ptree_bad_path>> &error) {
        std::cerr << error.what();
    }
}



void databaseConfig::readConfigFile(const std::string& file) {
    pt::ptree databaseTree;
    pt::read_xml(file, databaseTree);

    try {
        database.hostName = databaseTree.get<std::string>("database.hostName");
        database.dataBaseName = databaseTree.get<std::string>("database.dataBaseName");
        database.userName = databaseTree.get<std::string>("database.userName");
        database.password = databaseTree.get<std::string>("database.password");

    } catch (boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::property_tree::ptree_bad_path>> &error) {
        std::cerr << error.what();
    }
}