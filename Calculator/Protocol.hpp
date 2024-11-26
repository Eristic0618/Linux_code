#pragma once
//协议部分

#include <iostream>
#include <string>

const std::string space_sep = " ";
const std::string req_sep = "\n";

std::string Encode(std::string& content) //添加报头
{
    std::string package = std::to_string(content.size()); //将有效载荷长度作为报头
    package += req_sep;
    package += content;
    package += req_sep;
    //"len"\n"x op y"\n
    return package;
}

bool Decode(std::string& package, std::string *content) //去除报头
{
    size_t pos = package.find(req_sep); //找到第一个\n
    if(pos == std::string::npos)
        return false;

    std::string len_str = package.substr(0, pos);
    std::size_t len = std::stoi(len_str); //取出有效载荷长度

    std::size_t total_len = len_str.size() + len + 2; //整个请求长度
    if(package[total_len-1] != '\n')
        return false; //total_len-1应该为请求结尾\n处，如果不是则非法

    *content = package.substr(pos + 1, len);
    //若package包含多个请求，去掉第一个
    package.erase(0, total_len);
    
    return true;
}

class Request // 请求
{
public:
    Request(int x, int y, char op)
        : x_(x), y_(y), op_(op)
    {}

    Request()
    {}

public:
    bool Serialize(std::string* out) // 序列化
    {
        // 构建有效载荷 "x op y"
        std::string msg = std::to_string(x_);
        msg += space_sep;
        msg += op_;
        msg += space_sep;
        msg += std::to_string(y_);
        *out = msg;

        return true;
    }

    bool Deserialize(const std::string& in) // 反序列化
    {
        size_t lpos = in.find(space_sep); //找到第一个空格
        if(lpos == std::string::npos)
            return false;
        std::string x = in.substr(0, lpos); //取出x

        size_t rpos = in.rfind(space_sep); //找到第二个空格

        if(rpos == std::string::npos)
            return false;
        std::string y = in.substr(rpos + 1); //取出y
        if(lpos + 2 != rpos) //运算符长度为1，如果无误rpos应该在lpos后面两位
            return false;

        op_ = in[lpos + 1]; //取出运算符
        x_ = std::stoi(x);
        y_ = std::stoi(y);
        return true;
    }

    void Print()
    {
        std::cout << "new request: " << x_ << " " << op_ << " " << y_ << std::endl;
    }

public:
    int x_;   // 操作数1
    int y_;   // 操作数2
    char op_; // 运算符
};

class Response // 响应
{
public:
    Response(int result = 0, int code = 0)
        : result_(result), code_(code)
    {}

public:
    bool Serialize(std::string *out) // 序列化
    {
        // 构建响应正文 "result code"
        std::string msg = std::to_string(result_);
        msg += space_sep;
        msg += std::to_string(code_);
        *out = msg;

        return true;
    }

    bool Deserialize(const std::string& in) // 反序列化
    {
        size_t pos = in.find(space_sep); //找到第一个空格
        if(pos == std::string::npos)
            return false;
        std::string result = in.substr(0, pos); //取出result
        std::string code = in.substr(pos + 1); // 取出code

        result_ = std::stoi(result);
        code_ = std::stoi(code);
        return true;
    }

    void Print()
    {
        std::cout << "calculate finish, result: " << result_ << " , code: " << code_ << std::endl;
    }

public:
    int result_; // 运算结果
    int code_;   // 错误码
};