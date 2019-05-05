///////////////////////////
// �����:  Kalinin Pavel //
// ����:   Encoder.cpp   //
// ������: 04.05.2019    //
///////////////////////////

#include "Encoder.h"
#include "ui_Encoder.h"
#include <fstream>

Encoder::Encoder(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    // ������� �����, ���������� ������ "�����������" � "������������"
    connect(DataButton, SIGNAL(clicked()), this, SLOT(ActivateButtons()));

    // ������� ����
    connect(EncodeButton, SIGNAL(clicked()), this, SLOT(Encode()));

    // �������������� ����
    connect(DecodeButton, SIGNAL(clicked()), this, SLOT(Decode()));
}

// ������� �����, ������������/�������������� ������
void Encoder::ActivateButtons()
{
    // ������ ��� ������ � ������������� ������
    std::ifstream notEnFInOut(NOT_ENCODE_FILE_NAME);

    // ������ ��� ������ � ������������� ������
    std::ifstream enFInOut(ENCODE_FILE_NAME);

    // ��������� �������� �� ������������� ����
    if (!notEnFInOut.is_open())
    {
        // ������� ������������� ����
        std::ofstream notEnFOut(NOT_ENCODE_FILE_NAME);

        // ���� ���� �� �������� - �����
        if (!notEnFOut.is_open())
        {
            return;
        };

        notEnFOut.close();
    };

    // ��������� �������� �� ������������� ����
    if (!enFInOut.is_open())
    {
        // ������� ������ ���� ��� ������������� ����������
        std::ofstream enFOut(ENCODE_FILE_NAME);

        // ���� ���� �� �������� - �����
        if (!enFOut.is_open())
        {
            return;
        };

        enFOut.close();
    };

    // ������������� ������ "������� ����� ������"
    DataButton->setEnabled(false);

    // ��������� ������ "�����������"
    EncodeButton->setEnabled(true);

    // ��������� ������ "������������"
    DecodeButton->setEnabled(true);

    // ��������� �����
    enFInOut.close();
    notEnFInOut.close();
}

// ����������� ����
void Encoder::Encode()
{
    // ����������� ������ "�����������"
    EncodeButton->setEnabled(false);

    // ����������� ������ "������������"
    DecodeButton->setEnabled(false);

    // ������ ��� ������ � ������������� ������
    std::ifstream notEnFInOut(NOT_ENCODE_FILE_NAME);

    // ������ ��� ������ � ������������� ������
    std::ofstream enFInOut(ENCODE_FILE_NAME);

    // ��������
    if (!notEnFInOut.is_open())
    {
        return;
    };
    if (!enFInOut.is_open())
    {
        return;
    };

    char buf[100];

    notEnFInOut.seekg(0, std::ios::beg);
    int i = 0;
    for (;!notEnFInOut.eof();++i)
    {
        buf[i] = notEnFInOut.get();
    };
    --i;
    buf[i] = 'x';

    int temp = i;

    for (int i = 0; i <= temp; ++i)
    {
        enFInOut << buf[i];
    };

    // ��������� ������ "�����������"
    EncodeButton->setEnabled(true);

    // ��������� ������ "������������"
    DecodeButton->setEnabled(true);
}

// ������������ ����
void Encoder::Decode()
{
    // ����������� ������ "�����������"
    EncodeButton->setEnabled(false);

    // ����������� ������ "������������"
    DecodeButton->setEnabled(false);

    // ������ ��� ������ � ������������� ������
    std::ofstream notEnFInOut(NOT_ENCODE_FILE_NAME);

    // ������ ��� ������ � ������������� ������
    std::ifstream enFInOut(ENCODE_FILE_NAME);

    // ��������
    if (!notEnFInOut.is_open())
    {
        return;
    };
    if (!enFInOut.is_open())
    {
        return;
    };

    char buf[100];

    enFInOut.seekg(0, std::ios::beg);
    int i = 0;
    for (; !enFInOut.eof(); ++i)
    {
        buf[i] = enFInOut.get();
    };
    --i;
    --i;
    int temp = i;

    if (buf[i] == '0')
    {
        for (int i = 0; i < temp; ++i)
        {
            notEnFInOut << buf[i];
        };
    };

    // ��������� ������ "�����������"
    EncodeButton->setEnabled(true);

    // ��������� ������ "������������"
    DecodeButton->setEnabled(true);
}
