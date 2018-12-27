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
#include <stdlib.h>

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
int sCurr;
int sNext;

int lettersCount = 7;
char letters[] = "IJLOSTZ";
int shapes[][4][4][4] =
        {
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
            },
            {
                    {
                            {1, 0, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {1, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            {
                    {
                            {0, 0, 1, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 1, 0},
                            {1, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {1, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            {
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            {
                    {
                            {0, 1, 1, 0},
                            {1, 1, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {0, 1, 1, 0},
                            {1, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {1, 0, 0, 0},
                            {1, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            {
                    {
                            {0, 1, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {1, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            {
                    {
                            {1, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 1, 0},
                            {0, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {1, 1, 0, 0},
                            {1, 0, 0, 0},
                            {0, 0, 0, 0}
                    }
            }
        };

int is_occupied(int row, int col)
{
    if(field[row][col] == 1)
        return 1;


    if(row >= sRow && row < sRow+4 && col >= sCol && col < sCol+4)
        return shapes[sCurr][rotation][row-sRow][col-sCol];

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

void generate_next_shape(void)
{
    sNext = (sCurr+1) % lettersCount; // @todo rand() is not working

    LCD_clear();
    LCD_append_string("Next shape: ");
    LCD_append_char(letters[sNext]);
}

void shape_spawn(void)
{
    // @todo Check for collide, if so, then end the game

    rotation = 0;
    sRow = 0;
    sCol = 3;

    sCurr = sNext;
    generate_next_shape();
}


void shape_rotate(void)
{
    int attempts;
    int row;
    int col;
    int newRotation;
    int failed = 1; // 1 to pass the first iteration

    for(attempts = 1; attempts < 4 && failed; attempts++)
    {
        failed = 0;
        newRotation = (rotation+attempts) % 4;

        for(row = 0; row < 4 && !failed; row++)
        {
            for(col = 0; col < 4 && !failed; col++)
            {
                if(shapes[sCurr][newRotation][row][col] == 1)
                    if(field[sRow+row][sCol+col])
                        failed = 1;
            }
        }
    }

    if(!failed)
        rotation = newRotation;
}


int shape_move(int rowChange, int colChange)
{
    int row;
    int col;

    for(row = 0; row < 4; row++)
    {
        for(col = 0; col < 4; col++)
        {
            if(shapes[sCurr][rotation][row][col] == 1)
                if(field[sRow+row+rowChange][sCol+col+colChange])
                    return 0;
        }
    }

    sRow += rowChange;
    sCol += colChange;

    return 1;
}

void shape_static()
{
    int row;
    int col;

    for(row = 0; row < 4; row++)
    {
        for(col = 0; col < 4; col++)
        {
            if(shapes[sCurr][rotation][row][col] == 1)
                field[sRow+row][sCol+col] = 1;
        }
    }
}

void row_clear(int row)
{
    int col;

    for(col = 1; col < 9; col++)
    {
        field[row][col] = 0;
    }
}

int row_is_full(int row)
{
    int col;
    for(col = 1; col < 9; col++)
    {
        if(field[row][col] == 0)
            return 0;
    }
    return 1;
}

int row_copy(int destRow, int srcRow)
{
    int col;
    for(col = 1; col < 9; col++)
    {
        field[destRow][col] = field[srcRow][col];
    }
}

void field_drop(int row)
{
    for(; row >= 0; row--)
    {
        if(row-1 >= 0)
        {
            row_copy(row, row-1);
            row_clear(row-1);
        }
        else
            row_clear(row);
    }
}

void field_clear()
{
    int row;

    for(row = 7; row >= 0; row--)
    {
        if (row_is_full(row))
        {
            field_drop(row);
            row++; // Check this row again because it changed
        }
    }
}

void shape_down()
{
    if(!shape_move(1, 0))
    {
        shape_static();
        field_clear();
        shape_spawn();
    }
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
                shape_rotate();
                break;

            case '5':
                shape_down();
                break;
            case '4':
                shape_move(0, -1);
                break;
            case '6':
                shape_move(0, 1);
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

    generate_next_shape();
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
