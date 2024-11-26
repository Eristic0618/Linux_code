#pragma once
// �ṩ���㹦��
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
        default: // �Ƿ������
            resp.code_ = Nodef_Op;
            break;
        }
        return resp;
    }

    std::string Calculate(std::string &package) // ����
    {
        std::string content;
        bool flag = Decode(package, &content); // ȡ����Ч�غ�
        if (!flag)
            return "";

        Request req;
        flag = req.Deserialize(content); // �����л�
        if (!flag)
            return "";

        Response resp = CalculateHelper(req); // ��ʼ����

        std::string msg;
        flag = resp.Serialize(&msg); // ��Ӧ���л�
        if (!flag)
            return "";

        msg = Encode(msg); // ��ӱ�ͷ
        return msg;
    }

    ~Calculator()
    {}
};
