#pragma once
// 提供计算功能
#include <string>
#include "Protocol.hpp"

enum
{
    Div_Zero = 2,
    Mod_Zero,
    Nodef_Op
};

class Calculator
{
public:
    Calculator()
    {
    }

    Response CalculateHelper(Request &req)
    {
        Response resp;
        switch (req.op_)
        {
        case '+':
            resp.result_ = req.x_ + req.y_;
            break;
        case '-':
            resp.result_ = req.x_ - req.y_;
            break;
        case '*':
            resp.result_ = req.x_ * req.y_;
            break;
        case '/':
        {
            if (req.y_ == 0)
                resp.code_ = Div_Zero;
            else
                resp.result_ = req.x_ / req.y_;
        }
        break;
        case '%':
        {
            if (req.y_ == 0)
                resp.code_ = Mod_Zero;
            else
                resp.result_ = req.x_ % req.y_;
        }
        break;
        default: // 非法运算符
            resp.code_ = Nodef_Op;
            break;
        }
        return resp;
    }

    std::string Calculate(std::string &package) // 计算
    {
        std::string content;
        bool flag = Decode(package, &content); // 取出有效载荷
        if (!flag)
            return "";

        Request req;
        flag = req.Deserialize(content); // 反序列化
        if (!flag)
            return "";

        Response resp = CalculateHelper(req); // 开始计算

        std::string msg;
        flag = resp.Serialize(&msg); // 响应序列化
        if (!flag)
            return "";

        msg = Encode(msg); // 添加报头
        return msg;
    }

    ~Calculator()
    {}
};
