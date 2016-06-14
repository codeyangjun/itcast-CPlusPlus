#!/bin/bash
#breakout
while  :

do
        echo -n "Enter any num[1...5]:"
        read num
        case $num in
        1|2|3|4|5)
                echo "You enter a num between 1 and 5"
                ;;
        *)
                echo -n "Wrong num, continue (y/n)?: "
                read IS_CONTINUE
                case $IS_CONTINUE in
                        y|yes|Yes)
                                continue
                                ;;
                        *)
                                break;
                                ;;
                esac
        esac
done








