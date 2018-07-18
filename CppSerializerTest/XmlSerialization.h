#pragma once

std::ostream& xmlOut(std::ostream& os, const Record& rec)
{
    os << rec.name << "=\"";
    switch (rec.type)
    {
    case Record::DataType::BOOL:
    {
        os << (*(static_cast<bool*>(rec.pData)) ? "true" : "false");
        break;
    }
    case Record::DataType::INT:
    {
        os << *static_cast<int*>(rec.pData);
        break;
    }
    case Record::DataType::DBL:
    {
        os << *static_cast<double*>(rec.pData);
        break;
    }
    case Record::DataType::STR:
    {
        //std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
        //os << myconv.to_bytes(*static_cast<std::wstring*>(rec.pData));
        os << (*static_cast<std::string*>(rec.pData));
        break;
    }
    default:
        break;
    }
    os << '\"';
    return os;
}

std::ostream& xmlOut(std::ostream& os, const std::vector<Record>& records)
{
    os << "\t";
    std::string sep;
    for (const auto& rec : records)
    {
        os << sep;
        xmlOut(os, rec);
        sep = ' ';
    }
    return os;
}



