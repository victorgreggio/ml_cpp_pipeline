#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace csv
{
    class CsvWriter;

    inline static CsvWriter &endrow(CsvWriter &file);
    inline static CsvWriter &flush(CsvWriter &file);

    class CsvWriter
    {
    public:
        CsvWriter() = delete;
        
        CsvWriter(const std::string filename, const std::string separator = ",")
            : m_fs(), m_is_first(true), m_separator(separator), m_escape_seq("\""), m_special_chars("\"")
        {
            m_fs.exceptions(std::ios::failbit | std::ios::badbit);
            m_fs.open(filename);
        }

        CsvWriter(const std::string filename, const std::string separator, const std::string escape_seq, const std::string special_chars)
            : m_fs(), m_is_first(true), m_separator(separator), m_escape_seq(escape_seq), m_special_chars(special_chars)
        {
            m_fs.exceptions(std::ios::failbit | std::ios::badbit);
            m_fs.open(filename);
        }

        ~CsvWriter()
        {
            flush();
            m_fs.close();
        }

        void flush()
        {
            m_fs.flush();
        }

        void endrow()
        {
            m_fs << std::endl;
            m_is_first = true;
        }

        CsvWriter &operator<<(CsvWriter &(*val)(CsvWriter &))
        {
            return val(*this);
        }

        CsvWriter &operator<<(const char *val)
        {
            return write(escape(val));
        }

        CsvWriter &operator<<(const std::string &val)
        {
            return write(escape(val));
        }

        template <typename T>
        CsvWriter &operator<<(const T &val)
        {
            return write(val);
        }

    private:
        std::ofstream m_fs;
        bool m_is_first;
        const std::string m_separator;
        const std::string m_escape_seq;
        const std::string m_special_chars;

        template <typename T>
        CsvWriter &write(const T &val)
        {
            if (!m_is_first)
            {
                m_fs << m_separator;
            }
            else
            {
                m_is_first = false;
            }
            m_fs << val;
            return *this;
        }

        std::string escape(const std::string &val)
        {
            std::ostringstream result;
            result << '"';
            std::string::size_type to, from = 0u, len = val.length();
            while (from < len &&
                   std::string::npos != (to = val.find_first_of(m_special_chars, from)))
            {
                result << val.substr(from, to - from) << m_escape_seq << val[to];
                from = to + 1;
            }
            result << val.substr(from) << '"';
            return result.str();
        }
    };

    inline static CsvWriter &endrow(CsvWriter &file)
    {
        file.endrow();
        return file;
    }

    inline static CsvWriter &flush(CsvWriter &file)
    {
        file.flush();
        return file;
    }
} // namespace csv

#endif