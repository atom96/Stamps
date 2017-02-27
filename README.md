Stamps - program for stamps collection. </br>
</br>
Authors: Arkadiusz Tomczak and Ryszard Poklewski-Koziełł</br>
Version: 1.0</br>
Date: 13-10-2016</br>
Language:</br>
 - Variable names: English
 - Comments: English

Program reads list of stamps in format:</br>
```
Name-of-stamp price-of-stamp Year-of-manufactoring Name-of-country-or-post-office
```

Names can contain white caracters, between each part of stamp description may be any kind of white caracters, not only spces.</br>
For example:
```
One Cent 930 1856  British Guiana
```

Price may contain comma or dot, both are accepted.

After reading list, user may ask about stamps in specific range (years). </br> For example:
```
1700 1972
```
Program has input validation based on regular expressions, gives error message to stderr if row is not valid.
