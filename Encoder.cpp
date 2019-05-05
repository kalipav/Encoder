///////////////////////////
// Автор:  Kalinin Pavel //
// Файл:   Encoder.cpp   //
// Создан: 04.05.2019    //
///////////////////////////

#include "Encoder.h"
#include "ui_Encoder.h"
#include <fstream>

Encoder::Encoder(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    // создаем файлы, активируем кнопки "Зашифровать" и "Расшифровать"
    connect(DataButton, SIGNAL(clicked()), this, SLOT(ActivateButtons()));

    // шифруем файл
    connect(EncodeButton, SIGNAL(clicked()), this, SLOT(Encode()));

    // расшифровываем файл
    connect(DecodeButton, SIGNAL(clicked()), this, SLOT(Decode()));
}

// создать файлы, активировать/деактивировать кнопки
void Encoder::ActivateButtons()
{
    // объект для работы с нешифрованным файлом
    std::ifstream notEnFInOut(NOT_ENCODE_FILE_NAME);

    // объект для работы с зашифрованным файлом
    std::ifstream enFInOut(ENCODE_FILE_NAME);

    // проверяем открылся ли нешифрованный файл
    if (!notEnFInOut.is_open())
    {
        // создаем нешифрованный файл
        std::ofstream notEnFOut(NOT_ENCODE_FILE_NAME);

        // если файл не открылся - выход
        if (!notEnFOut.is_open())
        {
            return;
        };

        notEnFOut.close();
    };

    // проверяем открылся ли зашифрованный файл
    if (!enFInOut.is_open())
    {
        // создаем пустой файл для зашифрованной информации
        std::ofstream enFOut(ENCODE_FILE_NAME);

        // если файл не открылся - выход
        if (!enFOut.is_open())
        {
            return;
        };

        enFOut.close();
    };

    // заблокировать кнопку "Создать файлы данных"
    DataButton->setEnabled(false);

    // активация кнопки "Зашифровать"
    EncodeButton->setEnabled(true);

    // активация кнопки "Расшифровать"
    DecodeButton->setEnabled(true);

    // закрываем файлы
    enFInOut.close();
    notEnFInOut.close();
}

// зашифровать файл
void Encoder::Encode()
{
    // деактивация кнопки "Зашифровать"
    EncodeButton->setEnabled(false);

    // деактивация кнопки "Расшифровать"
    DecodeButton->setEnabled(false);

    // объект для работы с нешифрованным файлом
    std::ifstream notEnFInOut(NOT_ENCODE_FILE_NAME);

    // объект для работы с зашифрованным файлом
    std::ofstream enFInOut(ENCODE_FILE_NAME);

    // проверка
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

    // активация кнопки "Зашифровать"
    EncodeButton->setEnabled(true);

    // активация кнопки "Расшифровать"
    DecodeButton->setEnabled(true);
}

// расшифровать файл
void Encoder::Decode()
{
    // деактивация кнопки "Зашифровать"
    EncodeButton->setEnabled(false);

    // деактивация кнопки "Расшифровать"
    DecodeButton->setEnabled(false);

    // объект для работы с нешифрованным файлом
    std::ofstream notEnFInOut(NOT_ENCODE_FILE_NAME);

    // объект для работы с зашифрованным файлом
    std::ifstream enFInOut(ENCODE_FILE_NAME);

    // проверка
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

    // активация кнопки "Зашифровать"
    EncodeButton->setEnabled(true);

    // активация кнопки "Расшифровать"
    DecodeButton->setEnabled(true);
}
