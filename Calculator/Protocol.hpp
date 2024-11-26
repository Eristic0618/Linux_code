#pragma once
//Э�鲿��

#include <iostream>
#include <string>

const std::string space_sep = " ";
const std::string req_sep = "\n";

std::string Encode(std::string& content) //��ӱ�ͷ
{
    std::string package = std::to_string(content.size()); //����Ч�غɳ�����Ϊ��ͷ
    package += req_sep;
    package += content;
    package += req_sep;
    //"len"\n"x op y"\n
    return package;
}

bool Decode(std::string& package, std::string *content) //ȥ����ͷ
{
    size_t pos = package.find(req_sep); //�ҵ���һ��\n
    if(pos == std::string::npos)
        return false;

    std::string len_str = package.substr(0, pos);
    std::size_t len = std::stoi(len_str); //ȡ����Ч�غɳ���

    std::size_t total_len = len_str.size() + len + 2; //�������󳤶�
    if(package[total_len-1] != '\n')
        return false; //total_len-1Ӧ��Ϊ�����β\n�������������Ƿ�

    *content = package.substr(pos + 1, len);
    //��package�����������ȥ����һ��
    package.erase(0, total_len);
    
    return true;
}

class Request // ����
{
public:
    Request(int x, int y, char op)
        : x_(x), y_(y), op_(op)
    {}

    Request()
    {}

public:
    bool Serialize(std::string* out) // ���л�
    {
        // ������Ч�غ� "x op y"
        std::string msg = std::to_string(x_);
        msg += space_sep;
        msg += op_;
        msg += space_sep;
        msg += std::to_string(y_);
        *out = msg;

        return true;
    }

    bool Deserialize(const std::string& in) // �����л�
    {
        size_t lpos = in.find(space_sep); //�ҵ���һ���ո�
        if(lpos == std::string::npos)
            return false;
        std::string x = in.substr(0, lpos); //ȡ��x

        size_t rpos = in.rfind(space_sep); //�ҵ��ڶ����ո�

        if(rpos == std::string::npos)
            return false;
        std::string y = in.substr(rpos + 1); //ȡ��y
        if(lpos + 2 != rpos) //���������Ϊ1���������rposӦ����lpos������λ
            return false;

        op_ = in[lpos + 1]; //ȡ�������
        x_ = std::stoi(x);
        y_ = std::stoi(y);
        return true;
    }

    void Print()
    {
        std::cout << "new request: " << x_ << " " << op_ << " " << y_ << std::endl;
    }

public:
    int x_;   // ������1
    int y_;   // ������2
    char op_; // �����
};

class Response // ��Ӧ
{
public:
    Response(int result = 0, int code = 0)
        : result_(result), code_(code)
    {}

public:
    bool Serialize(std::string *out) // ���л�
    {
        // ������Ӧ���� "result code"
        std::string msg = std::to_string(result_);
        msg += space_sep;
        msg += std::to_string(code_);
        *out = msg;

        return true;
    }

    bool Deserialize(const std::string& in) // �����л�
    {
        size_t pos = in.find(space_sep); //�ҵ���һ���ո�
        if(pos == std::string::npos)
            return false;
        std::string result = in.substr(0, pos); //ȡ��result
        std::string code = in.substr(pos + 1); // ȡ��code

        result_ = std::stoi(result);
        code_ = std::stoi(code);
        return true;
    }

    void Print()
    {
        std::cout << "calculate finish, result: " << result_ << " , code: " << code_ << std::endl;
    }

public:
    int result_; // ������
    int code_;   // ������
};