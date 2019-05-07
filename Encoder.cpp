///////////////////////////
// Автор:  Kalinin Pavel //
// Файл:   Encoder.cpp   //
// Создан: 04.05.2019    //
///////////////////////////

#include "Encoder.h"
#include "ui_Encoder.h"
#include <fstream>
#include <deque>
#include <QString>
#include <string>

Encoder::Encoder(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    // проверяем файлы, активируем поле ключа
    connect(dataButton, SIGNAL(clicked()), this, SLOT(CheckFiles()));

    // разблокировка кнопок "Зашифровать" и "Расшифровать"
    connect(keyLineEdit, SIGNAL(textChanged(QString)), this, SLOT(ActivateButtons(QString)));

    // шифруем файл
    connect(encodeButton, SIGNAL(clicked()), this, SLOT(Encode()));

    // расшифровываем файл
    connect(decodeButton, SIGNAL(clicked()), this, SLOT(Decode()));
}

// проверить файлы
void Encoder::CheckFiles()
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
    dataButton->setEnabled(false);

    // активируем лейбл "ключ"
    keyLabel->setEnabled(true);

    // активируем строку ввода ключа
    keyLineEdit->setEnabled(true);

    // закрываем файлы
    enFInOut.close();
    notEnFInOut.close();
}

// активация клавиш "расшифровать" и "зашифровать"
// [in] const QString& - строка
void Encoder::ActivateButtons(const QString& rSTR)
{
    // если длина введенного ключа меньше минимальной разрешенной длины ключа
    if (rSTR.length() < KEY_LENGTH)
    {
        // заблокировать кнопки
        encodeButton->setEnabled(false);
        decodeButton->setEnabled(false);
    }
    else
    {
        // разблокировать кнопки
        encodeButton->setEnabled(true);
        decodeButton->setEnabled(true);
    }
}

// зашифровать файл
void Encoder::Encode()
{
    // деактивация кнопки "Зашифровать"
    encodeButton->setEnabled(false);

    // деактивация кнопки "Расшифровать"
    decodeButton->setEnabled(false);

    // деактивация строки ввода
    keyLineEdit->setEnabled(false);

    // объект для работы с нешифрованным файлом
    std::ifstream notEnFIn(NOT_ENCODE_FILE_NAME);

    // объект для работы с зашифрованным файлом
    std::ofstream enFOut(ENCODE_FILE_NAME);

    // проверка
    if (!notEnFIn.is_open())
    {
        return;
    };
    if (!enFOut.is_open())
    {
        return;
    };

    // создаем вектор нешифрованных символов
    std::vector <char>* pDataVec = new std::vector <char>;

    // создаем вектор шифрованных символов
    std::vector <char>* pEncodeDataVec = new std::vector <char>;

    // перемещаем указатель на начало файла
    notEnFIn.seekg(0, std::ios::beg);

    // считываем файл
    while (!notEnFIn.eof())
    {
        pDataVec->push_back(notEnFIn.get());
    };

    // удаляем последний символ (символ конца файла)
    pDataVec->pop_back();

    // копируем ключ
    const QString KEY = keyLineEdit->text();

    // преобразуем ключ в std::string
    const std::string STR_KEY = KEY.toStdString();

    // проходим по вектору
    for (std::vector <char>::iterator iter = pDataVec->begin(); iter != pDataVec->end(); ++iter)
    {
        // буфер, содержащий символ нешифрованного файла
        Byte buf;

        // копируем в буфер текущий элемент
        buf.sym = *iter;

        // шифруем, проходим по всем символам ключа
        for (int i = 0; i < STR_KEY.length(); ++i)
        {
            // временная переменная, хранящая символ
            Byte temp = buf;

            // инвертируем побитно
            buf.sym = ~temp.sym;

            // временная переменная для хранения символа ключа
            Byte tempKeySym;
            tempKeySym.sym = STR_KEY[i];

            // флаг (увеличение следующего старшего разряда на +1)
            bool flag;

            // копируем содержимое буфера во временную переменную
            temp = buf;

            // обнуляем флаг
            flag = false;

            // суммирование
            buf.symBits.bit0 = Summator(temp.symBits.bit0, tempKeySym.symBits.bit0, flag);
            buf.symBits.bit1 = Summator(temp.symBits.bit1, tempKeySym.symBits.bit1, flag);
            buf.symBits.bit2 = Summator(temp.symBits.bit2, tempKeySym.symBits.bit2, flag);
            buf.symBits.bit3 = Summator(temp.symBits.bit3, tempKeySym.symBits.bit3, flag);
            buf.symBits.bit4 = Summator(temp.symBits.bit4, tempKeySym.symBits.bit4, flag);
            buf.symBits.bit5 = Summator(temp.symBits.bit5, tempKeySym.symBits.bit5, flag);
            buf.symBits.bit6 = Summator(temp.symBits.bit6, tempKeySym.symBits.bit6, flag);
            buf.symBits.bit7 = Summator(temp.symBits.bit7, tempKeySym.symBits.bit7, flag);
        };

            // копируем символ в вектор с шифрованными данными в 16-тиричной системе
        // переменная для хранения символа 16-тиричного кода, записываем старшую тетраду
        char tempChar = ConvertIn16(buf.symBits.bit7,
                                    buf.symBits.bit6,
                                    buf.symBits.bit5,
                                    buf.symBits.bit4);

        // записываем старшую тетраду в вектор
        pEncodeDataVec->push_back(tempChar);

        // переменная для хранения символа 16-тиричного кода, записываем младшую тетраду
        tempChar = ConvertIn16(buf.symBits.bit3,
                               buf.symBits.bit2,
                               buf.symBits.bit1,
                               buf.symBits.bit0);

        // записываем младшую тетраду в вектор
        pEncodeDataVec->push_back(tempChar);
    };

        // записываем содержимое вектора в файл
    // устанавливаем позицию для записи
    enFOut.seekp(0, std::ios::beg);

    // проходим по вектору с шифрованными данными
    for (std::vector <char>::iterator iter = pEncodeDataVec->begin(); iter != pEncodeDataVec->end(); ++iter)
    {
        enFOut.put(*iter);
    };

    // закрываем файлы
    notEnFIn.close();
    enFOut.close();

    // очищаем векторы
    pDataVec->clear();
    pEncodeDataVec->clear();

    // освобождаем память
    delete pDataVec;
    delete pEncodeDataVec;

    // разблокировать кнопку "Создать файлы данных"
    dataButton->setEnabled(true);
}

// расшифровать файл
void Encoder::Decode()
{
    // деактивация кнопки "Зашифровать"
    encodeButton->setEnabled(false);

    // деактивация кнопки "Расшифровать"
    decodeButton->setEnabled(false);

    // деактивация строки ввода
    keyLineEdit->setEnabled(false);

    // объект для работы с нешифрованным файлом
    std::ofstream notEnFOut(NOT_ENCODE_FILE_NAME);

    // объект для работы с зашифрованным файлом
    std::ifstream enFIn(ENCODE_FILE_NAME);

    // проверка
    if (!notEnFOut.is_open())
    {
        return;
    };
    if (!enFIn.is_open())
    {
        return;
    };

    // создаем вектор нешифрованных символов
    std::vector <char>* pDataVec = new std::vector <char>;

    // создаем вектор шифрованных символов
    std::vector <char>* pEncodeDataVec = new std::vector <char>;

    // перемещаем указатель на начало файла
    enFIn.seekg(0, std::ios::beg);

    // считываем файл
    while (!enFIn.eof())
    {
        pEncodeDataVec->push_back(enFIn.get());
    };

    // удаляем последний символ (символ конца файла)
    pEncodeDataVec->pop_back();

    // копируем ключ
    const QString KEY = keyLineEdit->text();

    // преобразуем ключ в std::string
    const std::string STR_KEY = KEY.toStdString();

    //#####################################################
    // символ 0х1 (для обратного кода)
    Byte one;
    one.symBits.bit7 = false;
    one.symBits.bit6 = false;
    one.symBits.bit5 = false;
    one.symBits.bit4 = false;
    one.symBits.bit3 = false;
    one.symBits.bit2 = false;
    one.symBits.bit1 = false;
    one.symBits.bit0 = true;
    //#####################################################

    // проходим по вектору
    for (std::vector <char>::iterator iter = pEncodeDataVec->begin(); iter != pEncodeDataVec->end(); ++iter)
    {
        // буфер, содержащий символ шифрованного файла
        Byte buf;

            // копируем в буфер текущий байт
        // считываем старшую тетраду из вектора
        char seniorTetrad = *iter;

        // следующий символ вектора (младшая тетрада)
        ++iter;

        // считываем младшую тетраду из вектора
        char juniorTetrad = *iter;

        // устанавливаем последовательность бит для буфера согласно тетрадам
        SetBitsFromTetrad(seniorTetrad, juniorTetrad, buf);

        // шифруем, проходим по всем символам ключа в обратном порядке
        for (int i = STR_KEY.length() - 1; i >= 0; --i)
        {
            // временная переменная для последующих операций
            Byte temp;

            // временная переменная для хранения символа ключа
            Byte tempKeySym;
            tempKeySym.sym = STR_KEY[i];

            // флаг (увеличение следующего старшего разряда на +1)
            bool flag;

                // дополнительный код для символа ключа
            // копия символа ключа
            temp = tempKeySym;

            // инверсия
            tempKeySym.sym = ~temp.sym;

            // копия инвертированного символа ключа
            temp = tempKeySym;

            // обнуляем флаг
            flag = false;

            // сложение инвертированного ключа и символа 0х1
            tempKeySym.symBits.bit0 = Summator(temp.symBits.bit0, one.symBits.bit0, flag);
            tempKeySym.symBits.bit1 = Summator(temp.symBits.bit1, one.symBits.bit1, flag);
            tempKeySym.symBits.bit2 = Summator(temp.symBits.bit2, one.symBits.bit2, flag);
            tempKeySym.symBits.bit3 = Summator(temp.symBits.bit3, one.symBits.bit3, flag);
            tempKeySym.symBits.bit4 = Summator(temp.symBits.bit4, one.symBits.bit4, flag);
            tempKeySym.symBits.bit5 = Summator(temp.symBits.bit5, one.symBits.bit5, flag);
            tempKeySym.symBits.bit6 = Summator(temp.symBits.bit6, one.symBits.bit6, flag);
            tempKeySym.symBits.bit7 = Summator(temp.symBits.bit7, one.symBits.bit7, flag);

                // суммирование шифрованного символа и дополнительного кода символа ключа
            // копируем содержимое буфера во временную переменную
            temp = buf;

            // обнуляем флаг
            flag = false;

            // суммирование
            buf.symBits.bit0 = Summator(temp.symBits.bit0, tempKeySym.symBits.bit0, flag);
            buf.symBits.bit1 = Summator(temp.symBits.bit1, tempKeySym.symBits.bit1, flag);
            buf.symBits.bit2 = Summator(temp.symBits.bit2, tempKeySym.symBits.bit2, flag);
            buf.symBits.bit3 = Summator(temp.symBits.bit3, tempKeySym.symBits.bit3, flag);
            buf.symBits.bit4 = Summator(temp.symBits.bit4, tempKeySym.symBits.bit4, flag);
            buf.symBits.bit5 = Summator(temp.symBits.bit5, tempKeySym.symBits.bit5, flag);
            buf.symBits.bit6 = Summator(temp.symBits.bit6, tempKeySym.symBits.bit6, flag);
            buf.symBits.bit7 = Summator(temp.symBits.bit7, tempKeySym.symBits.bit7, flag);

                // инвертируем полученную последовательность
            // копируем содержимое буфера во временную переменную
            temp = buf;

            // инвертируем побитно
            buf.sym = ~temp.sym;
        };

        // записываем расшифрованный символ в вектор
        pDataVec->push_back(buf.sym);
    };

        // записываем расшифрованную информацию вектора в файл
    // устанавливаем позицию для записи
    notEnFOut.seekp(0, std::ios::beg);

    // проходим по вектору с расшифрованными данными
    for (std::vector <char>::iterator iter = pDataVec->begin(); iter != pDataVec->end(); ++iter)
    {
        notEnFOut.put(*iter);
    };

    // закрываем файлы
    notEnFOut.close();
    enFIn.close();

    // очищаем векторы
    pDataVec->clear();
    pEncodeDataVec->clear();

    // освобождаем память
    delete pDataVec;
    delete pEncodeDataVec;

    // разблокировать кнопку "Создать файлы данных"
    dataButton->setEnabled(true);
}

// Конвертация 4-х бит в символ 16-тиричной системы
//[in]  const bool& - старший разряд
//[in]  const bool& - ...
//[in]  const bool& - ...
//[in]  const bool& - младший разряд
//[out] char        - символ
char Encoder::ConvertIn16(const bool& rFIRST, const bool& rSECOND, const bool& rTHIRD, const bool& rFOURTH)
{
    if (rFIRST == false && rSECOND == false && rTHIRD == false && rFOURTH == false)
    {
        return '0';
    }
    else if (rFIRST == false && rSECOND == false && rTHIRD == false && rFOURTH == true)
    {
        return '1';
    }
    else if (rFIRST == false && rSECOND == false && rTHIRD == true && rFOURTH == false)
    {
        return '2';
    }
    else if (rFIRST == false && rSECOND == false && rTHIRD == true && rFOURTH == true)
    {
        return '3';
    }
    else if (rFIRST == false && rSECOND == true && rTHIRD == false && rFOURTH == false)
    {
        return '4';
    }
    else if (rFIRST == false && rSECOND == true && rTHIRD == false && rFOURTH == true)
    {
        return '5';
    }
    else if (rFIRST == false && rSECOND == true && rTHIRD == true && rFOURTH == false)
    {
        return '6';
    }
    else if (rFIRST == false && rSECOND == true && rTHIRD == true && rFOURTH == true)
    {
        return '7';
    }
    else if (rFIRST == true && rSECOND == false && rTHIRD == false && rFOURTH == false)
    {
        return '8';
    }
    else if (rFIRST == true && rSECOND == false && rTHIRD == false && rFOURTH == true)
    {
        return '9';
    }
    else if (rFIRST == true && rSECOND == false && rTHIRD == true && rFOURTH == false)
    {
        return 'a';
    }
    else if (rFIRST == true && rSECOND == false && rTHIRD == true && rFOURTH == true)
    {
        return 'b';
    }
    else if (rFIRST == true && rSECOND == true && rTHIRD == false && rFOURTH == false)
    {
        return 'c';
    }
    else if (rFIRST == true && rSECOND == true && rTHIRD == false && rFOURTH == true)
    {
        return 'd';
    }
    else if (rFIRST == true && rSECOND == true && rTHIRD == true && rFOURTH == false)
    {
        return 'e';
    }
    else if (rFIRST == true && rSECOND == true && rTHIRD == true && rFOURTH == true)
    {
        return 'f';
    }
}

// устанавливаем последовательность бит для буфера согласно тетрадам
// [in]     const char& - старшая тетрада
// [in]     const char& - младшая тетрада
// [in/out] Byte&       - содержит последовательность бит
void Encoder::SetBitsFromTetrad(const char& rSENIOR_TETRAD, const char& rJUNIOR_TETRAD, Byte& rBuf)
{
    if (rSENIOR_TETRAD == '0')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == '1')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = true;
    }
    else if (rSENIOR_TETRAD == '2')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == '3')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = true;
    }
    else if (rSENIOR_TETRAD == '4')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == '5')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = true;
    }
    else if (rSENIOR_TETRAD == '6')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == '7')
    {
        rBuf.symBits.bit7 = false;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = true;
    }
    else if (rSENIOR_TETRAD == '8')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == '9')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = true;
    }
    else if (rSENIOR_TETRAD == 'a')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == 'b')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = false;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = true;
    }
    else if (rSENIOR_TETRAD == 'c')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == 'd')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = false;
        rBuf.symBits.bit4 = true;
    }
    else if (rSENIOR_TETRAD == 'e')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = false;
    }
    else if (rSENIOR_TETRAD == 'f')
    {
        rBuf.symBits.bit7 = true;
        rBuf.symBits.bit6 = true;
        rBuf.symBits.bit5 = true;
        rBuf.symBits.bit4 = true;
    }

    if (rJUNIOR_TETRAD == '0')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == '1')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = true;
    }
    else if (rJUNIOR_TETRAD == '2')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == '3')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = true;
    }
    else if (rJUNIOR_TETRAD == '4')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == '5')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = true;
    }
    else if (rJUNIOR_TETRAD == '6')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == '7')
    {
        rBuf.symBits.bit3 = false;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = true;
    }
    else if (rJUNIOR_TETRAD == '8')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == '9')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = true;
    }
    else if (rJUNIOR_TETRAD == 'a')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == 'b')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = false;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = true;
    }
    else if (rJUNIOR_TETRAD == 'c')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == 'd')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = false;
        rBuf.symBits.bit0 = true;
    }
    else if (rJUNIOR_TETRAD == 'e')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = false;
    }
    else if (rJUNIOR_TETRAD == 'f')
    {
        rBuf.symBits.bit3 = true;
        rBuf.symBits.bit2 = true;
        rBuf.symBits.bit1 = true;
        rBuf.symBits.bit0 = true;
    }
}

// Сумматор
// [in]     const bool& - первое слагаемое
// [in]     const bool& - второе слагаемое
// [in/out] bool&       - флаг
// [out]    bool        - результат суммирования
bool Encoder::Summator(const bool& rFIRST, const bool& rSECOND, bool& rFlag)
{
    if (rFIRST == false && rSECOND == false && rFlag == false)
    {
        return false;
    }
    else if ((rFIRST == false && rSECOND == true  && rFlag == false) ||
             (rFIRST == true  && rSECOND == false && rFlag == false) ||
             (rFIRST == false && rSECOND == false && rFlag == true ))
    {
        rFlag = false;
        return true;
    }
    else if ((rFIRST == true  && rSECOND == true  && rFlag == false) ||
             (rFIRST == false && rSECOND == true  && rFlag == true ) ||
             (rFIRST == true  && rSECOND == false && rFlag == true ))
    {
        rFlag = true;
        return false;
    }
    else if (rFIRST == true && rSECOND == true && rFlag == true)
    {
        return true;
    }
}
