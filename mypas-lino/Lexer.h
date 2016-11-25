/*
 * Lexer.h
 *
 *  Created on: 6 de jan de 2016
 *      Author: mario
 */

#ifndef LEXER_H_
#define LEXER_H_

#define MAX_ID_LENGTH 10240
extern char     lexeme[MAX_ID_LENGTH + 1];
extern int      lexcursor;
extern int		line_count;

#endif /* LEXER_H_ */
