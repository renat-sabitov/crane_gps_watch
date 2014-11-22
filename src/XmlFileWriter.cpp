// Copyright (C) 2014 mru@sisyphus.teil.cc
//
// linux client for crane gps watch, runtastic gps watch.
//



#include <stack>
#include <stdexcept>
#include <cassert>
#include <iostream>
#include <sstream>


#include "XmlFileWriter.hpp"

unsigned char* get_str(const std::string& str) {
    return (unsigned char*) str.c_str();
}

XmlFileWriter::XmlFileWriter() : wf(nullptr), w(nullptr) {
}

XmlFileWriter::~XmlFileWriter() {
    // XXX shouldn't happen
    if (isOpen()) {
        close();
    }
}

void XmlFileWriter::open(const std::string& filename) {
    std::cerr << "XmlFileWriter: writing to " << filename << std::endl;
    wf = fopen(filename.c_str(), "w");
    w = genxNew(NULL, NULL, NULL);
}

bool XmlFileWriter::isOpen() {
    return w != nullptr;
}

void XmlFileWriter::close() {
    if (isOpen()) {
        //throw std::runtime_error("Protocol mismatch: closing already closed XML document");
    }
    genxDispose(w);
    fclose(wf);
    wf = nullptr;
    w = nullptr;
}

void XmlFileWriter::startDocument(const std::string& encoding) {
    genxStartDocFile(w, wf);
}
void XmlFileWriter::endDocument() {
    genxEndDocument(w);
}

void XmlFileWriter::startElement(const std::string& name) {
    genxStartElementLiteral(w, NULL, get_str(name));
    stack.push(name);
}

void XmlFileWriter::endElement(const std::string& name) {
    genxEndElement(w);
    if (name.size()) {
        if (name != stack.top()) throw std::runtime_error("XML Tag mismatch (expected a <" + stack.top() + ">, got a <" + name +">)");
    }
    stack.pop();
}

void XmlFileWriter::writeAttribute(const std::string& name, const std::string& value) {
    genxAddAttributeLiteral(w, NULL, get_str(name), get_str(value));
}

void XmlFileWriter::writeElement(const std::string& name, const std::string& value) {
    startElement(name);
    genxAddText(w, get_str(value));
    endElement(name);
}