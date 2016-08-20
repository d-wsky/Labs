/* 
 * Copyright (c) 2016, Denis Vasilkovskii
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *      - Redistributions of source code must retain the above copyright
 *        notice, this list of conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright
 *        notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *      - Neither the name of the OINPE NRNU MEPhI nor the
 *        names of its contributors may be used to endorse or promote products
 *        derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * interrupts.h
 *
 * Created: 10-Aug-16 00:45:03
 *  Author: Denis Vasilkovskii
 *
 *   About: ��������� ��� ������ � ������������.
 */ 


#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/*! \brief ����� ����� ��� ��������.*/
typedef enum {
    IRQ_PIN_0,          /*!< ����� INT0 (PD0).*/
    IRQ_PIN_1,          /*!< ����� INT1 (PD1).*/
    IRQ_PIN_2,          /*!< ����� INT2 (PD2).*/
    IRQ_PIN_3,          /*!< ����� INT3 (PD3).*/
    IRQ_PIN_4,          /*!< ����� INT4 (PD4).*/
    IRQ_PIN_5,          /*!< ����� INT5 (PD5).*/
    IRQ_PIN_6,          /*!< ����� INT6 (PD6).*/
    IRQ_PIN_7,          /*!< ����� INT7 (PD7).*/
    IRQ_SOURCES_AMOUNT, /*!< ��� �������� ������������� ��� ����������� �������������
                             ������ ����������.*/
} IrqPin_t;

/*! \brief �������� ���������������� �������-����������� ����������.*/
typedef void (*Isr_t)(void);

/*! \brief ����� ���� �������, �� ������� ���������� �������.*/
typedef enum {
    IRQ_MODE_LOW     = 0, /*!< ���������� ��������� �� �����, ���� �� �����
                               ������ �������� ����������.*/
    IRQ_MODE_CHANGE  = 1, /*!< ���������� ��������� ��� ����� ���������
                               ����������� ������ �� �����.*/
    IRQ_MODE_FALLING = 2, /*!< ���������� ��������� ��� ������ (�����������)
                               ������ ������� �� �����.*/
    IRQ_MODE_RISING  = 3, /*!< ���������� ��������� �� �������� (����������)
                               ������ ������� �� �����.*/
} IrqMode_t;

/*! \brief ����� ������� ������������ ����������� � ������ �� ����������
����������. 

\details ��� ����, ����� ������������ � ���������� ����������, �����������
����������� ��� ������� ��������� ��� ���� �������. ����� (�������� 1) �
��� ������� (�������� 3) ����� ������� � ������� �������������� \ref IrqPin_t
� \ref IrqMode_t ��������������. � �������� ������� ��������� ����������
������� �������, ������� ������: �� ����� ������� ���������� � �� ����������
������. ������� �������, ������� ����:

\code
void interrupt_handler(void);
\endcode

���������� �����������, ��� ������ ������� ����� ������� � ����� ������,
����� ���������� ����� ���������������� �������������� �� ����. �������
�������, ��� ���������� ����������, ����� \a interrupt_handler ����
����� ������ � ��������� ����������, �� ���� ����������� �����������
������������� ����� ������ ������� � �������. ������� �� �������������
������� ������ ��������� ���, ������� ����� ���������� ������ ������ �������.

����� ����� ��� ����������, ������������ ������ ����������� ����������, 
��������� � �������� ������ volatile, ��������� � ���� ������ �����������
����������� �� ������ ��������� �� �� ���������.

\param pin: �����, ������� ����� ����������� �� ������� ��� ��������� 
������������ � ��� ����������.
\param isr:    ������� ���������� ����������, ������� ������ �������������� �
���������������� ���������. ��� ������� ������ �� ����� ������� ����������
� ������ ���������� void.
\param mode: ���������� ��� �������, �� ������� ��������� �������. ��.
\ref IrqMode_t.
*/
void interruptAttach(IrqPin_t pin, Isr_t isr, IrqMode_t mode);

/*! \brief ���������� ����������� ���������� �� �����, � ��� �� ������
������� ������� �� ���������� �� ��������� �����.

\param �����, ���������� �� ������� ������� ���������.
*/
void interruptDeattach(IrqPin_t pin);

#endif /* INTERRUPTS_H */