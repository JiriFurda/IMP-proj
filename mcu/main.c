/*******************************************************************************
   main.c: LCD + keyboard demo
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$


*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>

char last_ch; //naposledy precteny znak
char char_cnt;

int field[9][10] =
        {
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };

//int shape;
int rotation;
int sRow;
int sCol;

int shape[4][4][4] =
        {
                {
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
                        {0, 0, 0, 0},
                        {0, 0, 0, 0}
                },
                {
                        {0, 0, 1, 0},
                        {0, 0, 1, 0},
                        {0, 0, 1, 0},
                        {0, 0, 1, 0}
                },
                {
                        {0, 0, 0, 0},
                        {0, 0, 0, 0},
                        {1, 1, 1, 1},
                        {0, 0, 0, 0}
                },
                {
                        {0, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 1, 0, 0},
                        {0, 1, 0, 0}
                }
        };

int is_occupied(int row, int col)
{
    if(field[row][col] == 1)
        return 1;


    if(row >= sRow && row < sRow+4 && col >= sCol && col < sCol+4)
        //return 1;
        return shape[rotation][row-sRow][col-sCol];

    return 0;
}

void field_print(void)
{
    term_send_crlf();

    int row;
    int col;



    for(row = 0; row < 8; row++)
    {
        for(col = 1; col < 9; col++)
        {
            if(is_occupied(row, col))
                term_send_str("#");
            else
                term_send_str("_");
        }

        term_send_crlf();
    }
}

void shape_spawn(void)
{
    rotation = 0;
    sRow = 0;
    sCol = 3;
}

/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
}


/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle()
{
    char ch;
    ch = key_decode(read_word_keyboard_4x4());
    if (ch != last_ch)
    {
        last_ch = ch;

        switch(ch)
        {
            case 0:
                break;

            case '2':
                rotation = (rotation+1) % 4;
                break;

            case '5':
                sRow++;
                break;
            case '4':
                sCol--;
                break;
            case '6':
                sCol++;
                break;
        }

        field_print();


        /*
        if (ch != 0)
        {

            term_send_crlf();
            term_send_str("Na klavesnici byla stisknuta klavesa \'");
            term_send_char(ch);
            term_send_char('\'');
            term_send_crlf();
            term_send_str(" >");



            if (char_cnt == 16) {
                LCD_clear();
                LCD_append_string("KB demo ");
                char_cnt = 0;
            }
            LCD_append_char(ch);
            if (char_cnt > 7) {
                LCD_rotate();
            }
            char_cnt++;
        }
         */
    }
    return 0;
}



/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
    return CMD_UNKNOWN;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
    LCD_init();
    LCD_clear();
    LCD_append_string("KB demo ");
}


/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
    unsigned int cnt = 0;
    char_cnt = 0;
    last_ch = 0;

    initialize_hardware();
    keyboard_init();

    set_led_d6(1);                       // rozsviceni D6
    set_led_d5(1);                       // rozsviceni D5

    shape_spawn();
    field_print();

    while (1)
    {
        delay_ms(10);
        cnt++;
        if (cnt > 50)
        {
            cnt = 0;
            flip_led_d6();                   // negace portu na ktere je LED
        }

        keyboard_idle();                   // obsluha klavesnice
        terminal_idle();                   // obsluha terminalu
    }
}
