///////////////////////////
// �����:  Kalinin Pavel //
// ����:   Encoder.h     //
// ������: 04.05.2019    //
///////////////////////////

#ifndef ENCODER_H
#define ENCODER_H

#include <QDialog>
#include "ui_Encoder.h"

#define ENCODE_FILE_NAME "EncodeData.dat" // ��� �������������� �����
#define NOT_ENCODE_FILE_NAME "Data.dat" // ��� �������������� �����

class Encoder : public QDialog, public Ui::Encoder
{
    Q_OBJECT
    
public:
    Encoder(QWidget *parent = 0);

public slots:
    // ������� �����, ������������/�������������� ������
    void ActivateButtons();

    // ����������� ����
    void Encode();

    // ������������ ����
    void Decode();
};

#endif // ENCODER_H
