///////////////////////////
// Автор:  Kalinin Pavel //
// Файл:   Encoder.h     //
// Создан: 04.05.2019    //
///////////////////////////

#ifndef ENCODER_H
#define ENCODER_H

#include <QDialog>
#include "ui_Encoder.h"

#define ENCODE_FILE_NAME     "EncodeData.dat" // имя зашифрованного файла
#define NOT_ENCODE_FILE_NAME "Data.dat"       // имя нешифрованного файла
#define KEY_LENGTH           8                // минимальная длина ключа

// биты символа
union Byte
{
    char sym;
    struct Bits
    {
        bool bit0 : 1;
        bool bit1 : 1;
        bool bit2 : 1;
        bool bit3 : 1;
        bool bit4 : 1;
        bool bit5 : 1;
        bool bit6 : 1;
        bool bit7 : 1;
    } symBits;
};

class Encoder : public QDialog, public Ui::Encoder
{
    Q_OBJECT
    
public:
    Encoder(QWidget *parent = 0);

public slots:
    // проверить файлы
    void CheckFiles();

    // активация клавиш "расшифровать" и "зашифровать"
    void ActivateButtons(const QString&);

    // зашифровать файл
    void Encode();

    // расшифровать файл
    void Decode();

private:
    // Сумматор
    bool Summator(const bool&, const bool&, bool&);

    // Конвертация 4-х бит в символ 16-тиричной системы
    char ConvertIn16(const bool&, const bool&, const bool&, const bool&);

    // устанавливаем последовательность бит для буфера согласно тетрадам
    void SetBitsFromTetrad(const char&, const char&, Byte&);
};

#endif // ENCODER_H
