#include "xmlparser.h"
#include <fstream>
using namespace std;

XmlNode::XmlNode()
    : index(-1), loadSuccess(0)
{
    
}

XmlNode::XmlNode(istream &in)
    : index(-1), loadSuccess(0)
{
    load(in);
}

void XmlNode::load(istream &in)
{
    bool goUp;
    char c = 0;
    
    do {
        goUp = false;
        
        tagName.clear();
        attr.clear();
        
        while(in.good()) {
            
            char c = in.get();
            
            if(c == '<')
                break;
        }
        
        if(!in.good())
            return;
        
        c = in.get();
        
        while(isspace(c)) {
            
            if(!in.good())
                return;
            
            c = in.get();
        }
        
        if(c == '!') {
            
            // Comment
            if(in.get() == '-' && in.get() == '-') {
                
                while(in.good())
                    if(in.get() == '-'
                        && in.get() == '-'
                            && in.get() == '>')
                                break;
                
                if(!in.good())
                    return;
                
                goUp = true;
                continue;
            }
        }
        
    } while(goUp);
    
    bool endTag = false;
    
    if(c == '/') {
        
        endTag = true;
        tagName += '/';
        c = in.get();
    }
    
    while(isalnum(c) || c == '_') {
        
        tagName += c;
        
        c = in.get();
        
        if(!in.good())
            return;
    }
    
    string name;
    string value;
    char quoteType = 0;
    bool equal = false;
    bool quote = false;
    bool escape = false;
    
    char lastChar = 0;
    
    string *cur = &name;
    
    while(true) {
        
        bool done = false;
        
        if(escape) {
            
            *cur += c;
            escape = false;
        }
        else if(c == '\\') {
            
            escape = true;
        }
        else if(quote) {
            
            if(c == quoteType) {
                
                goto AttrDone;
            }
            else {
                
                *cur += c;
            }                
        }
        else if(c == '"' || c == '\'') {
            
            quoteType = c;
            
            quote = true;
        }
        else if(c == '=' && !equal) {
            
            equal = true;
            cur = &value;
        }
        else if(isspace(c)) {
            
            if(equal)
                goto AttrDone;
            else if(cur->size())
                *cur += c;
        }
        else if(c == '/') {
            
            goto AttrDone;
        }
        else if(c == '>') {
            
            // Have kids?
            if(!endTag && lastChar != '/') {
                
                while(true) {
                    
                    XmlNode xmlNode(in);
                    
                    if(!xmlNode.loadSuccess)
                        break;
                    
                    if(xmlNode.tagName == string("/") + tagName)
                        break;
                    
                    kids.push_back(xmlNode);
                }
            }
            
            done = true;
            goto AttrDone;
        }
        else
            *cur += c;
        
goto Cont;
AttrDone:
        if(!name.empty()) {
            
            attr[name].str = value;
            
            name.clear();
            value.clear();
            
            cur = &name;
            
            equal = quote = escape = false;
        }
Cont:
        if(done)
            break;
        
        lastChar = c;
        c = in.get();
        
        if(!in.good())
            return;
    }
    
    loadSuccess = true;
}

static string helpSaveDoEscapes(string s, const string &escape = "\"\\")
{
    for(size_t pos = 0; pos < s.size(); ++pos)
        if(escape.find(s[pos]) != string::npos)
            s.insert(pos++, 1, '\\');
    
    return s;
}

static void helpSaveWork(const XmlNode *xmlNode, ostream &out, int indent = 0)
{
    out << string(indent * 4, ' ') << "<" << xmlNode->tagName;
    
    XmlNode::Attr::const_iterator itr = xmlNode->attr.begin();
    
    for(; itr != xmlNode->attr.end(); ++itr) {
        
        out << " " << itr->first << "=\""
            << helpSaveDoEscapes(itr->second.str)
            << "\"";
    }
    
    if(xmlNode->kids.empty())
        out << "/";
    
    out << ">\n";
    
    XmlNodes::const_iterator ntr = xmlNode->kids.begin();
    
    for(; ntr != xmlNode->kids.end(); ++ntr)
        helpSaveWork(&*ntr, out, indent + 1);
    
    if(!xmlNode->kids.empty())
        out << string(indent * 4, ' ')
            << "</" << xmlNode->tagName << ">\n";
}

void XmlNode::save(ostream &out) const
{
    helpSaveWork(this, out);
}

string XmlNode::getAttrStr(const string &key, const std::string &def) const
{
    if(attr.count(key))
        return attr.find(key)->second.str;
    
    return def;
}

const XmlNodes &loadXmlNodes(istream &in)
{
    static XmlNodes ret;
    
    ret.clear();
    
    while(true) {
        
        XmlNode xn(in);
        
        if(!xn.loadSuccess)
            break;
        
        ret.push_back(xn);
    }
    
    return ret;
}

const XmlNodes &loadXmlNodes(const string &filename)
{
    ifstream in(filename.c_str());
    
    return loadXmlNodes(in);
}

void saveXmlNodes(const XmlNodes &xmlNodes, ostream &out)
{
    XmlNodes::const_iterator itr = xmlNodes.begin();
    
    for(; itr != xmlNodes.end(); ++itr)
        itr->save(out);
}

void saveXmlNodes(const XmlNodes &xmlNodes, const std::string &filename)
{
    ofstream out(filename.c_str());
    
    saveXmlNodes(xmlNodes, out);
}


void printXmlNodeTree(const XmlNodes &xmlNodes, int indentSize, int indent, ostream &out)
{
    XmlNodes::const_iterator itr = xmlNodes.begin();
    
    for(; itr != xmlNodes.end(); ++itr) {
        
        out << string(indentSize * indent, ' ')
             << "<" << itr->tagName;
        
        XmlNode::Attr::const_iterator a = itr->attr.begin();
        
        for(; a != itr->attr.end(); ++a) {
            
            out << " " << a->first
                 << "=\"" << a->second.str << "\"";
        }
        
        if(itr->kids.empty())
            out << "/";
        
        out << ">\n";
        
        printXmlNodeTree(itr->kids, indentSize, indent + 1);
        
        if(!itr->kids.empty())
            out << string(indentSize * indent, ' ')
                 << "</" << itr->tagName << ">\n";
    }
}

Xml::NodePtrs Xml::find(XmlNodes &xmlNodes, const string &key, const string &val)
{
    NodePtrs ret;
    XmlNodes::iterator itr = xmlNodes.begin();
    
    for(; itr != xmlNodes.end(); ++itr) {
        
        XmlNode::Attr::iterator loc = itr->attr.find(key);
        
        if(loc != itr->attr.end())
            if(loc->second.str == val)
                ret.push_back(&*itr);
    }
    
    return ret;
}

Xml::ConstNodePtrs Xml::find(const XmlNodes &xmlNodes, const string &key, const string &val)
{
    ConstNodePtrs ret;
    XmlNodes::const_iterator itr = xmlNodes.begin();
    
    for(; itr != xmlNodes.end(); ++itr) {
        
        XmlNode::Attr::const_iterator loc = itr->attr.find(key);
        
        if(loc != itr->attr.end())
            if(loc->second.str == val)
                ret.push_back(&*itr);
    }
    
    return ret;
}

string Xml::escape(string str, const string &badChars, char escapeChar)
{
    /* Escape the escapeChar itself! */
    
    size_t loc = 0;
    
    while(loc < str.size()) {
        
        loc = str.find(escapeChar, loc);
        
        if(loc == string::npos)
            break;
        
        str.insert(loc, 1, escapeChar);
        
        loc += 2;
    }
    
    /* Now escape everything else... */
    for(string::const_iterator pos = badChars.begin(); pos != badChars.end(); ++pos) {
        
        if(*pos == escapeChar)
            continue;
        
        loc = 0;
        
        while(loc < str.size()) {
            
            loc = str.find(*pos, loc);
            
            if(loc == string::npos)
                break;
            
            str.insert(loc, 1, escapeChar);
            
            loc += 2;
        }
    }
    
    return str;
}
