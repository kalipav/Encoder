///////////////////////////
// Автор:  Kalinin Pavel //
// Файл:   Encoder.h     //
// Создан: 04.05.2019    //
///////////////////////////

#ifndef ENCODER_H
#define ENCODER_H

#include <QDialog>
#include "ui_Encoder.h"

#define ENCODE_FILE_NAME "EncodeData.dat" // имя зашифрованного файла
#define NOT_ENCODE_FILE_NAME "Data.dat" // имя нешифрованного файла

class Encoder : public QDialog, public Ui::Encoder
{
    Q_OBJECT
    
public:
    Encoder(QWidget *parent = 0);

public slots:
    // создать файлы, активировать/деактивировать кнопки
    void ActivateButtons();

    // зашифровать файл
    void Encode();

    // расшифровать файл
    void Decode();
};

#endif // ENCODER_H
