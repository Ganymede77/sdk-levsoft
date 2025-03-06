/* 
 * File:   CLEV_ColorRGB.h
 * Author: alunos
 *
 * Created on 14 de Setembro de 2012, 14:28
 */

#ifndef CLEV_COLORRGB_H
#define	CLEV_COLORRGB_H

/*!
 * Project: LegendColor
 * 
 * The class CLEV_ColorRGB returns the RGB's values (RED, GREEN, BLUE).
 * 
 */

class CLEV_ColorRGB 
{
protected:

    int red;
    int green;
    int blue;
    
public:
    CLEV_ColorRGB(int r = 255, int g = 255, int b = 255);/*!< Set the RGB's values. */
   // CLEV_ColorRGB(const CLEV_ColorRGB& orig);
    virtual ~CLEV_ColorRGB();
    
    int getRedCode();/*!< Returns the red color's value. */
    int getGreenCode();/*!< Returns the green color's value. */
    int getBlueCode();/*!< Returns the blue color's value. */  

    void setRedCode(int r);/*!< Set the red color's value. */
    void setGreenCode(int g);/*!< Set the green color's value. */
    void setBlueCode(int b);/*!< Set the blue color's value. */  
    
    void setRGB(int r, int g, int b);/*!< Set the RGB's values. */  
    
	CLEV_ColorRGB& operator =  (const CLEV_ColorRGB &a);
	bool operator == (const CLEV_ColorRGB &a);
	bool operator != (const CLEV_ColorRGB &a);
    
	long toLong()
	{
		//long cor = red;
		//cor = (cor << 8) + green;
		//cor = (cor << 8) + blue;
		//return cor;
		return red << 16 | green << 8 | blue;
	}
	void fromLong(long cor)
	{
		red = (cor >> 16) & 0xFF;
		green = (cor >> 8) & 0xFF;
		blue = cor & 0xFF;
	}
};

#endif	/* CLEV_COLORRGB_H */

