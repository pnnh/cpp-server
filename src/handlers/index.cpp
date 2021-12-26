//
// Created by ubuntu on 12/25/21.
//

#include "index.h"
#include <iostream>
#include <fstream>
#include <boost/beast/core/error.hpp>
#include <boost/iostreams/copy.hpp>
#include "../utils/mime.h"

void HandleIndex(boost::beast::http::response<boost::beast::http::dynamic_body> &response_) {
    //response_.set(boost::beast::http::field::content_type, "text/html");
    std::ifstream infile;
    infile.open("static/index.html");

//    boost::beast::ostream(response_.body())
//            << "<html>\n"
//            << "<head><title>Current time</title></head>\n"
//            << "<body>\n"
//            << "<h1>Current time</h1>\n"
//            << "<p>The current time is "
//            << " seconds since the epoch.</p>\n"
//            << "</body>\n"
//            << "</html>\n";


    std::string path = "static/index.html";
    //处理请求
//    boost::beast::error_code ec;
//    boost::beast::http::file_body::value_type body;
//    body.open(path.c_str(), boost::beast::file_mode::scan, ec);
//
//    if (ec == boost::beast::errc::no_such_file_or_directory) {
//        // not found
//    }
    //std::ifstream ifs("output", ios::binary);
//    boost::iostreams::copy(infile,
//                           response_.body());


   std::string full_path = "static/index.html";
//
//    boost::beast::http::file_body::value_type file;
//    boost::beast::error_code ec;
//    file.open(
//            full_path.c_str(),
//            boost::beast::file_mode::read,
//            ec);
//    if(ec)
//    {
//        std::cout <<"File not found\r\n";
//        return;
//    }

    response_.result(boost::beast::http::status::ok);
    response_.keep_alive(false);
    response_.set(boost::beast::http::field::server, "Beast");
    response_.set(boost::beast::http::field::content_type, mime_type(std::string(full_path)));
    response_.content_length(100);

    boost::beast::ostream(response_.body()) << infile.rdbuf();
}