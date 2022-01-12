#include "Net.h"
#include "Cell.h"
#include "Line.h"
#include <algorithm>

namespace Netlist {

Net::Net(Cell* owner, const std ::string& name, Term ::Type type)
    : owner_(owner)
    , name_(name)
    , id_(owner->newNetId())
    , type_(type)
{
    owner_->add(this);
}

Net::Net(Instance* inst, const std ::string& name, Term ::Type type)
    : name_(name)
    , type_(type)
{
}
Net::~Net()
{
}
Cell* Net::getCell() const { return owner_; }
const std ::string& Net::getName() const { return name_; }
unsigned int Net::getId() const { return id_; }
Term ::Type Net::getType() const { return type_; }
Node* Net::getNode(size_t id) const { return nodes_[id]; }
const std ::vector<Node*>& Net::getNodes() const { return nodes_; }
size_t Net::getFreeNodeId() const
{
    for (size_t i = 0; i < nodes_.size(); ++i)
        if (nodes_[i] == nullptr)
            return i;
    return nodes_.size();
}

void Net::add(Node* node)
{
    if (node) {
        size_t id = node->getId();
        if (id == Node::noid) {
            id = getFreeNodeId();
            node->setId(id);
        }
        if (id < nodes_.size()) {
            if (nodes_[id] != nullptr) {
                std::cerr << "[ERROR] in Net<" << name_ << "node id:" << id
                          << " overwritten." << std::endl;
                nodes_[id]->setId(Node::noid);
            }
            nodes_[id] = node;

        } else {
            for (size_t i = nodes_.size(); i < id; ++i)
                nodes_.push_back(nullptr);

            nodes_.emplace_back(node);
        }
    }
}

bool Net::remove(Node* node)
{
    auto it = find(nodes_.begin(), nodes_.end(), node);
    if (it == nodes_.end())
        return false;

    nodes_.erase(it);
    return true;
}

void Net::add(Line* line)
{
    if (line)
        lines_.push_back(line);
}

bool Net::remove(Line* line)
{
    if (line) {
        for (std::vector<Line*>::iterator il = lines_.begin(); il != lines_.end(); ++il) {
            if (*il == line) {
                lines_.erase(il);
                return true;
            }
        }
    }
    return false;
}

void Net::toXml(std::ostream& o)
{
    o << indent << "<net name=\"" << name_ << "\" type=\"" << convertT(type_) << "\">" << std::endl;
    ++indent;
    for (auto node : nodes_)
        if (node)
            node->toXml(o);

    for (auto line : lines_)
        if (line)
            line->toXml(o);

    --indent;
    o << indent << "</net>" << std::endl;
}

Net* Net::fromXml(Cell* cell, xmlTextReaderPtr reader)
{
    const xmlChar* netTag = xmlTextReaderConstString(reader, (const xmlChar*)"net");
    const xmlChar* nodeTag = xmlTextReaderConstString(reader, (const xmlChar*)"node");
    const xmlChar* lineTag = xmlTextReaderConstString(reader, (const xmlChar*)"line");
    const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
    Net* net = nullptr;
    if (netTag == nodeName) {
        std::string netName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
        std::string typeName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"type"));

        if (typeName.empty() or netName.empty()) {
            std::cerr << "[ERROR] \"name\" or \"type\" attribute missing in <net> tag (line:"
                      << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return nullptr;
        }
        Term::Type type;
        if (typeName == "External")
            type = Term::External;
        else
            type = Term::Internal;

        net = new Net(cell, netName, type);

        while (true) {

            int status = xmlTextReaderRead(reader);
            if (status != 1) {
                if (status != 0) {
                    std::cerr << "[ERROR] Net::fromXml(): Unexpected termination of the XML parser." << std::endl;
                }
                break;
            }

            switch (xmlTextReaderNodeType(reader)) {
            case XML_READER_TYPE_COMMENT:
            case XML_READER_TYPE_WHITESPACE:
            case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
                continue;
            }

            const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);

            if (nodeName == netTag) {
                if (xmlTextReaderNodeType(reader) != XML_READER_TYPE_END_ELEMENT)
                    std::cerr << "[ERROR] no closing </net> tag (line: "
                              << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
                break;
            }

            if (nodeName == nodeTag) {
                if (Node::fromXml(net, reader))
                    continue;
            } else if (nodeName == lineTag)
                if (Line::fromXml(net, reader))
                    continue;

            std::cerr << "[ERROR] Cell::fromXml(): Unknown or misplaced tag <" << nodeName
                      << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
            return net;
        }
    } else
        std::cerr << "[ERROR] Cell::fromXml(): Unknown or misplaced tag <" << nodeName
                  << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

    return net;
}
}