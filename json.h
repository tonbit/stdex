#ifndef CHRYSA_JSON_H_
#define CHRYSA_JSON_H_

#include "stdcc.h"
#include <map>
namespace stdex {
namespace json {

class Value
{
public:
    inline Value()
    {
        _type = TYPE_NULL;
    }

    inline Value(const Value &other)
    {
        _type = other._type;

        if (_type == TYPE_NUMBER)
            _number = other._number;
        else if (_type == TYPE_STRING)
            _string = other._string;
        else if (_type == TYPE_ARRAY)
            _array = other._array;
        else if (_type == TYPE_OBJECT)
            _object = other._object;
    }

    inline Value(Value &&other)
    {
        _type = other._type;

        if (_type == TYPE_NUMBER)
            _number = other._number;
        else if (_type == TYPE_STRING)
            _string = std::move(other._string);
        else if (_type == TYPE_ARRAY)
            _array = std::move(other._array);
        else if (_type == TYPE_OBJECT)
            _object = std::move(other._object);
    }

    inline Value(const std::map<string, string> &mapper)
    {
        _type = TYPE_OBJECT;

		for (auto &pair : mapper)
		{
			_object[pair.first] = pair.second;
		}
    }

    inline Value &operator=(const Value &other)
    {
        _type = other._type;

        if (_type == TYPE_NUMBER)
            _number = other._number;
        else if (_type == TYPE_STRING)
            _string = other._string;
        else if (_type == TYPE_ARRAY)
            _array = other._array;
        else if (_type == TYPE_OBJECT)
            _object = other._object;

        return *this;
    }

    inline Value &operator=(Value &&other)
    {
        _type = other._type;

        if (_type == TYPE_NUMBER)
            _number = other._number;
        else if (_type == TYPE_STRING)
            _string = std::move(other._string);
        else if (_type == TYPE_ARRAY)
            _array = std::move(other._array);
        else if (_type == TYPE_OBJECT)
            _object = std::move(other._object);

        return *this;
    }

    inline Value &operator=(const std::map<string, string> &mapper)
    {
        _type = TYPE_OBJECT;

		for (auto &pair : mapper)
		{
			_object[pair.first] = pair.second;
		}
        return *this;
    }

    inline Value &operator=(bool val)
    {
        _type = val ? TYPE_TRUE : TYPE_FALSE;
        return *this;
    }

    inline Value &operator=(i32 val)
    {
        _type = TYPE_NUMBER;
        _number = (double)val;
        return *this;
    }

    inline Value &operator=(i64 val)
    {
        _type = TYPE_NUMBER;
        _number = (double)val;
        return *this;
    }

    inline Value &operator=(f32 val)
    {
        _type = TYPE_NUMBER;
        _number = (double)val;
        return *this;
    }

    inline Value &operator=(f64 val)
    {
        _type = TYPE_NUMBER;
        _number = (double)val;
        return *this;
    }

    inline Value &operator=(size_t val)
    {
        _type = TYPE_NUMBER;
        _number = (double)val;
        return *this;
    }

    inline Value &operator=(const string &val)
    {
        _type = TYPE_STRING;
        _string = val;
        return *this;
    }

    inline Value &operator=(string &&val)
    {
        _type = TYPE_STRING;
        _string = std::move(val);
        return *this;
    }

    inline Value &operator=(const char *val)
    {
        _type = TYPE_STRING;
        _string = val;
        return *this;
    }

    inline Value &operator[](const string &s)
    {
    	assert(_type == TYPE_NULL || _type == TYPE_OBJECT);
        _type = TYPE_OBJECT;
        return _object[s];
    }

    inline Value &operator[](size_t i)
    {
        if (i >= _array.size())
            _array.resize(i+1);

		assert(_type == TYPE_NULL || _type == TYPE_ARRAY);
        _type = TYPE_ARRAY;
        return _array[i];
    }

    inline void set_true()
    {
        _type = TYPE_TRUE;
    }

    inline void set_false()
    {
        _type = TYPE_FALSE;
    }

    inline void set_number(double val)
    {
        _type = TYPE_NUMBER;
        _number = val;
    }

    inline bool get_bool() const
    {
        if (_type == TYPE_TRUE)
            return true;
        else
            return false;
    }

    inline i32 get_i32() const
    {
        return (i32)_number;
    }

    inline i64 get_i64() const
    {
        return (i64)_number;
    }

    inline f32 get_f32() const
    {
        return (f32)_number;
    }

    inline f64 get_f64() const
    {
        return (f64)_number;
    }

    inline string get_string() const
    {
        return _string;
    }

    inline std::vector<Value> get_array() const
    {
    	return _array;
    }

    inline std::map<string, Value> get_object() const
    {
    	return _object;
    }

    inline std::map<string, string> get_mapper() const
    {
    	std::map<string, string> map;

    	for (const auto &pair : _object)
    	{
   			const Value &value = pair.second;

			if (value._type == TYPE_STRING)
			{
				map[pair.first] = value._string;
			}
			else if (value._type == TYPE_NUMBER)
			{
				if (value._number - (i64)value._number > 0)
				{
					map[pair.first] = std::to_string(value._number);
				}
				else
				{
					map[pair.first] = std::to_string((i64)value._number);
				}
			}
			else if (value._type == TYPE_ARRAY)
			{
				string tmp;
				value.gen_array(tmp);
				map[pair.first] = std::move(tmp);
			}
			else if (value._type == TYPE_OBJECT)
			{
				string tmp;
				value.gen_array(tmp);
				map[pair.first] = std::move(tmp);
			}
			else if (value._type == TYPE_TRUE)
			{
				map[pair.first] = "true";
			}
			else if (value._type == TYPE_FALSE)
			{
				map[pair.first] = "false";
			}
			else if (value._type == TYPE_NULL)
			{
				map[pair.first] = "";
			}
		}

	    return std::move(map);
    }

    inline bool is_null() const
    {
    	return _type == TYPE_NULL;
    }

    inline bool is_number() const
    {
    	return _type == TYPE_NUMBER;
    }

    inline bool is_array() const
    {
    	return _type == TYPE_ARRAY;
    }

    inline bool is_object() const
    {
    	return _type == TYPE_OBJECT;
    }

    inline bool is_string() const
    {
    	return _type == TYPE_STRING;
    }

    //not support to escape control char
    string escape(const char *str, size_t len) const
    {
        string ret;
        ret.reserve(len);

        while (len > 0)
        {
            if (!iscntrl(str[0]) && str[0]!='"' && str[0]!='\\')
            {
                ret += str[0];
            }
            else
            {
                switch (str[0])
                {
                    case '\x08': ret += "\\b"; break;   //backspace
                    case '\x09': ret += "\\t"; break;   //tab
                    case '\x0a': ret += "\\n"; break;   //line feed
                    case '\x0c': ret += "\\f"; break;   //form feed
                    case '\x0d': ret += "\\r"; break;   //carriage return
                    case '\x22': ret += "\\\""; break;   //quotation mark
                    case '\x5c': ret += "\\\\"; break;   //reverse solidus
                }
            }
            ++str; --len;
        }

        return ret;
    }

    //not support to unescape control char
    string unescape(const char *str, int len)
    {
        string ret;
        ret.reserve(len);

        while (len > 0)
        {
            if (str[0] != '\\')
            {
                ret += str[0];
            }
            else if (len > 1)
            {
                switch (str[1])
                {
                    case '"': ret += '"'; break;        //quotation mark
                    case '\\': ret += '\\'; break;      //reverse solidus
                    case '/': ret += '/'; break;        //solidus
                    case 'b': ret += '\b'; break;       //backspace
                    case 'f': ret += '\f'; break;       //form feed
                    case 'r': ret += '\r'; break;       //carriage return
                    case 'n': ret += '\n'; break;       ////line feed
                    case 't': ret += '\t'; break;       //tab
                }
                ++str; --len;
            }
            ++str; --len;
        }

        return ret;
    }

    void gen_null(string &output) const
    {
        output += "null";
    }

    void gen_true(string &output) const
    {
        output += "true";
    }

    void gen_false(string &output) const
    {
        output += "false";
    }

    void gen_number(string &output) const
    {
    	if (_number - (i64)_number > 0)
    	{
			output += std::to_string(_number);
    	}
    	else
    	{
			output += std::to_string((i64)_number);
    	}
    }

    void gen_string(string &output) const
    {
        output += "\"";
        output += escape(_string.c_str(), _string.length());
        output += "\"";
    }

    void gen_array(string &output) const
    {
        output += "[";

        for (auto node : _array)
        {
            node.generate(output);
            output += ",";
        }

        output.back() = ']';
    }

    void gen_object(string &output) const
    {
        output += "{";

        for (auto &node : _object)
        {
            output += "\"";
            output += node.first;
            output += "\":";
            node.second.generate(output);
            output += ",";
        }

        output.back() = '}';
    }

    void generate(string &output) const
    {
        if (_type == TYPE_NUMBER)
        {
            gen_number(output);
        }
        else if (_type == TYPE_STRING)
        {
            gen_string(output);
        }
        else if (_type == TYPE_ARRAY)
        {
            gen_array(output);
        }
        else if (_type == TYPE_OBJECT)
        {
            gen_object(output);
        }
        else if (_type == TYPE_TRUE)
        {
            gen_true(output);
        }
        else if (_type == TYPE_FALSE)
        {
            gen_false(output);
        }
        else
        {
            gen_null(output);
        }
    }

    string serialize()
    {
        string tmp;
        tmp.reserve(4096);
        generate(tmp);
        return tmp;
    }

    inline int parse_ws(const char *str)
    {
        const char *cur = str;
        char c;

        while ((c = *cur) != '\0')
        {
            if (!isspace(c))
                break;

            ++cur;
        }

        return cur - str;
    }

    inline int parse_null(const char *str)
    {
        if (str[0] == 'n' && str[1] == 'u' && str[2] == 'l' && str[3] == 'l')
            return 4;

        return -1;
    }

    inline int parse_true(const char *str)
    {
        if (str[0] == 't' && str[1] == 'r' && str[2] == 'u' && str[3] == 'e')
            return 4;

        return -1;
    }

    inline int parse_false(const char *str)
    {
        if (str[0] == 'f' && str[1] == 'a' && str[2] == 'l' && str[3] == 's' && str[4] == 'e')
            return 5;

        return -1;
    }

    inline int parse_number(const char *str, double &val)
    {
        char *end;
        val = strtod(str, &end);
        return end - str;
    }

    inline int parse_string(const char *str)
    {
        const char *cur = str;
        char c;

        while ((c = *cur) != '\0')
        {
            if (c == '"')
                break;

            if (c == '\\')
                ++cur;

            ++cur;
        }

        return cur - str;
    }

    inline int parse_array(const char *str)
    {
        const char *cur = str;
        int off;

        if (*cur != '[')
            return -1;

        cur += 1;
        _type = TYPE_ARRAY;

        while (*cur)
        {
            string value;
            Value sub;

            off = parse_ws(cur);
            cur += off;

            if (*cur == 'n')
            {
                off = parse_null(cur);
                if (off < 0)
                    return -1;

                cur += off;
                _array.push_back(sub);
            }
            else if (*cur == 't')
            {
                off = parse_true(cur);
                if (off < 0)
                    return -1;

                sub.set_true();
                cur += off;
                _array.push_back(sub);
            }
            else if (*cur == 'f')
            {
                off = parse_false(cur);
                if (off < 0)
                    return -1;

                sub.set_false();
                cur += off;
                _array.push_back(sub);
            }
            else if (*cur == '"')
            {
                cur += 1;

                off = parse_string(cur);
                if (off < 0)
                    return -1;

                sub = unescape(cur, off);
                cur += off;
                _array.push_back(sub);

                if (*cur != '"')
                    return -1;

                cur += 1;
            }
            else if (*cur == '[')
            {
                off = sub.parse_array(cur);
                if (off < 0)
                    return -1;

                cur += off;
                _array.push_back(sub);
            }
            else if (*cur == '{')
            {
                off = sub.parse_object(cur);
                if (off < 0)
                    return -1;

                cur += off;
                _array.push_back(sub);
            }
            else if (::isdigit(*cur) || *cur == '+' || *cur == '-')
            {
                double tmp;

                off = parse_number(cur, tmp);
                if (off < 0)
                    return -1;

                sub.set_number(tmp);
                cur += off;
                _array.push_back(sub);
            }
            else
            {
                return -1;
            }

            off = parse_ws(cur);
            cur += off;

            if (*cur != ',')
                break;

            cur += 1;
        }

        if (*cur != ']')
            return -1;

        cur += 1;
        return cur - str;
    }

    int parse_object(const char *str)
    {
        const char *cur = str;
        int off;

        if (*cur != '{')
            return -1;

        cur += 1;
        _type = TYPE_OBJECT;

        while (*cur)
        {
            string label, value;
            Value sub;

            off = parse_ws(cur);
            cur += off;

            if (*cur != '"')
                return -1;

            cur += 1;

            off = parse_string(cur);
            if (off < 0)
                return -1;

            label.assign(unescape(cur, off));
            cur += off;

            if (*cur != '"')
                return -1;

            cur += 1;

            off = parse_ws(cur);
            cur += off;

            if (*cur != ':')
                return -1;

            cur += 1;

            off = parse_ws(cur);
            cur += off;

            if (*cur == 'n')
            {
                off = parse_null(cur);
                if (off < 0)
                    return -1;

                cur += off;
                _object[label] = sub;
            }
            else if (*cur == 't')
            {
                off = parse_true(cur);
                if (off < 0)
                    return -1;

                sub.set_true();
                cur += off;
                _object[label] = sub;
            }
            else if (*cur == 'f')
            {
                off = parse_false(cur);
                if (off < 0)
                    return -1;

                sub.set_false();
                cur += off;
                _object[label] = sub;
            }
            else if (::isdigit(*cur) || *cur == '+' || *cur == '-')
            {
                double tmp;

                off = parse_number(cur, tmp);
                if (off < 0)
                    return -1;

                sub.set_number(tmp);
                cur += off;
                _object[label] = sub;
            }
            else if (*cur == '"')
            {
                cur += 1;

                off = parse_string(cur);
                if (off < 0)
                    return -1;

                sub = unescape(cur, off);
                cur += off;
                _object[label] = sub;

                if (*cur != '"')
                    return -1;

                cur += 1;
            }
            else if (*cur == '[')
            {
                off = sub.parse_array(cur);
                if (off < 0)
                    return -1;

                cur += off;
                _object[label] = sub;
            }
            else if (*cur == '{')
            {
                off = sub.parse_object(cur);
                if (off < 0)
                    return -1;

                cur += off;
                _object[label] = sub;
            }
            else
            {
                return -1;
            }

            off = parse_ws(cur);
            cur += off;

            if (*cur != ',')
                break;

            cur += 1;
        }

        if (*cur != '}')
            return -1;

        cur += 1;
        return cur - str;
    }

    int parse(const char *str)
    {
        const char *cur = str;
        int off;

        off = parse_ws(cur);
        cur += off;

        if (*cur == '{')
        {
            _type = TYPE_OBJECT;

            off = parse_object(cur);
            if (off < 0)
                return -1;

            cur += off;
        }
        else if (*cur == '[')
        {
            _type = TYPE_ARRAY;

            off = parse_array(cur);
            if (off < 0)
                return -1;

            cur += off;
        }
        else
        {
            return -1;
        }

        off = parse_ws(cur);
        cur += off;

        return cur - str;
    }

    int deserialize(const string &input)
    {
        clear();
        return parse(input.c_str());
    }

    void clear()
    {
        _type = TYPE_NULL;
        _string.clear();
        _array.clear();
        _object.clear();
    }

private:
    enum Type
    {
        TYPE_NULL,
        TYPE_TRUE,
        TYPE_FALSE,
        TYPE_NUMBER,
        TYPE_STRING,
        TYPE_ARRAY,
        TYPE_OBJECT,
    };

    Type _type;
    double _number;
    string _string;
    std::vector<Value> _array;
    std::map<string, Value> _object;
};

}

}

using JsonValue = stdex::json::Value;

#endif //CHRYSA_JSON_H_
