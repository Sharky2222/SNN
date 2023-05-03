32 Bit Unsigned Integer Goldschmidt Division Architecture
by: Dylan D (3/23/23)

Top Level File: goldschmidt.v

Function:

A/B = Q

Dividend / Divisor = Quotient

A,B,Q: 32b

IN ORDER TO USE THE TOP LEVEL TEST BENCH, YOU NEED A TEXT FILE TO
COLLECT DATA. THE DEFAULT NAME IS "test_write.txt"

Notes: 

Architecture uses the goldschmidt algorithm to estimate a quotient. For
most cases, an accurate integer will be found within 3 clock cycles. In 
order to start a new division, the reset pin must be turned on and off 
exactly when or soon after a new A and B are loaded. 

Limitations:

In rare cases, the output can be up to 25% inaccurate. Future work needs
to be done in error analysis to mitigate this problem.