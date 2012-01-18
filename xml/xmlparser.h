#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

class XmlNode;
typedef std::vector<XmlNode> XmlNodes;

// A loose xml parser
class XmlNode {
public:
    class Typeless;
    typedef std::map<std::string, Typeless> Attr;
    
    std::string tagName;
    Attr attr;
    int index;
    
    XmlNodes kids;
    
    bool loadSuccess; // True if the tag is loaded and without error.
    
    XmlNode();
    XmlNode(std::istream&); // Reads in 1 tag.
    
    void load(std::istream&); // Reads in 1 tag.
    void save(std::ostream&) const;
    
    std::string getAttrStr(const std::string &key, const std::string &def = "") const;
    
    class Typeless {
    public:
        std::string str;
        bool conversionSuccess; // True if the last conversion was successful.
        
        template<typename T>
        T get(T t) const
        {
            std::istringstream in(str);
            
            in >> t;
            
            return t;
        }
        
        template<class T>
        void set(const T &t = T())
        {
            std::ostringstream os;
            
            os << t;
            
            str = os.str();
        }
    };
};

const XmlNodes &loadXmlNodes(std::istream&);
const XmlNodes &loadXmlNodes(const std::string &filename);
void saveXmlNodes(const XmlNodes &xmlNodes, std::ostream&);
void saveXmlNodes(const XmlNodes &xmlNodes, const std::string &filename);

void printXmlNodeTree(const XmlNodes&, int indentSize = 4, int indent = 0, std::ostream &out = std::cout);

namespace Xml {
    
    typedef std::vector<XmlNode*> NodePtrs;
    typedef std::vector<const XmlNode*> ConstNodePtrs;
    
    NodePtrs find(XmlNodes&, const std::string &attrKey, const std::string &attrVal);
    ConstNodePtrs find(const XmlNodes&, const std::string &attrKey, const std::string &attrVal);
    
    std::string escape(std::string, const std::string &badChars = "\"", char escapeChar = '\\');
}

#endif
