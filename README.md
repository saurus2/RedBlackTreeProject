# Product Shopping List Linux Application
## Environment
Language: C++
Environment: Linux/Shell
---
# How to use?
## Default Setting
* Reading a txt file for registeration of members.
* Information: memberNumber memberName phoneNumber addressX addressY level totalPrice

1. memberNumber: Member's number
2. memberName: Member's name
3. phoneNumber: Member's phone number
4. addressX: Member's address coordinate
5. addressY: Member's address coordinate
6. level: Level for calculating the purchase limit amount of goods
7. totalPrice: Total amount held by the member

## Commands 
* Q: Quit the program
* I memberNumber memberName phoneNumber addressX addressY: New membership command, enter I and enter membership information(Output results vary as member information is registered in duplicate)
* P memberNumber: Command P to output member information, if there is no member information, output an error message
* A number day productName payPrice: A member of the corresponding member number purchases a product at a specific price (processed according to the purchase limit of each member)
* R number many: Print out the details of the goods purchased by the member of the relevant member number as many as you want
* F: Outputs payment amounts from the top 3 people with high purchase payment amounts
---
