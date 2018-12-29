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

int gameOver = 0;

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

void game_over(void)
{
    LCD_clear();
    LCD_append_string("GAME OVER!");
    gameOver = 1;
}

void shape_spawn(void)
{
    int newRotation = 0;
    int newRow = 0;
    int newCol = 3;
    int newShape = sNext;

    int row;
    int col;

    for(row = 0; row < 4; row++)
    {
        for(col = 0; col < 4; col++)
        {
            if(shapes[newShape][newRotation][row][col] == 1 && field[newRow+row][newCol+col] == 1)
                return game_over();
        }
    }


    rotation = newRotation;
    sRow = newRow;
    sCol = newCol;

    sCurr = newShape;
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


void led_show_row(int led_col){

    unsigned char p1 = 0;
    unsigned char p3 = 0;

    /*
    led_col = led_col+1;




    p3 |= field[0][led_col];			// ROW 0 - Port 15 (P3M0)
    p3 |= (field[1][led_col] << 1);	// ROW 1 - Port 16 (P3M1)
    p3 |= (field[2][led_col] << 2);
    p3 |= (field[3][led_col] << 3);	// ROW 3 - Port 14 (P3M3)
    p1 |= (field[4][led_col] << 6);	// ROW 4 - Port 25 (P1M6)
    p1 |= (field[5][led_col] << 7);	// ROW 5 - Port 28 (P1M5)
    p3 |= (field[6][led_col] << 6);	// ROW 6 - Port 9 (P3M6)
    p3 |= (field[7][led_col] << 7);	// ROW 7 - Port 10 (P3M7)
    */

    //P3OUT = 1;
    //P3OUT = ~p3;
    //P1OUT = ~p1;


    int row[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    /*
    if(!row[0])
        p3 |= BIT0;

    if(!row[1])
        p3 |= BIT1;

    if(!row[3])
        p3 |= BIT3;
    */

    /*
    if(!row[4])
        p1 |= BIT6; cajk

    if(!row[5])
        p1 |= BIT5;
    */

    /*
     if(!row[6])
        p3 |= BIT6;

    if(!row[7])
        p3 |= BIT7;
    */

    //p3 |= BIT3;

    //P3OUT = p3;
    //P1OUT = BIT5;

    if(!row[1])
        p3 |= BIT1;

    if(!row[3])
        p3 |= BIT3;

    if(!row[6])
        p3 |= BIT6;

    if(!row[7])
        p3 |= BIT7;

    P3OUT = p3;
    P1OUT = p1;
}

int led_col = 0;
void led_show_col()
{
    led_show_row(7-led_col);

    P6OUT = ~(1 << (7-led_col));

    led_col = (led_col + 1) % 8;
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
    if(gameOver)
        return 0;

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
    LCD_append_string("Tetris");
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
    WDG_stop();
    keyboard_init();

    set_led_d6(1);                       // rozsviceni D6
    set_led_d5(1);                       // rozsviceni D5

    generate_next_shape();
    shape_spawn();
    field_print();

    TACTL = TASSEL_1 + MC_2;
    // CCR0 = 68;
    // CCTL0 = CCIE;

    P1DIR = 96; // 01100000
    P3DIR = 207; // 11001111
    P6DIR = 255; // 11111111

    while (1)
    {
        /*
        delay_ms(10);
        cnt++;
        if (cnt > 50)
        {
            cnt = 0;
            flip_led_d6();                   // negace portu na ktere je LED
        }
         */

        delay_ms(1);
        led_show_col();

        keyboard_idle();                   // obsluha klavesnice
        terminal_idle();                   // obsluha terminalu
    }
}

/*
interrupt (TIMERA0_VECTOR) Timer_A (void)
{
    led_show_col();
    CCR0 += 68; // plan next interrupt
}
*/

