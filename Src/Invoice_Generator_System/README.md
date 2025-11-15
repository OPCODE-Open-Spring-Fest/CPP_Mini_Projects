# Invoice Generator --- User Guide

This document explains how to run and use the Invoice Generator System.

------------------------------------------------------------------------

## Menu Options

    1. Create Invoice
    2. View Invoice by ID
    3. Show All Invoices
    4. Exit
    Enter choice:

------------------------------------------------------------------------

## 1. Create Invoice

### Steps:

1.  Enter customer name\
2.  Add items one by one:
    -   Enter item name\
    -   Enter quantity\
    -   Enter price\
3.  To finish adding items, **press ENTER on an empty item name**

### Example:

    Enter customer name: Aditya Gupta

    Enter item name (leave blank to finish): Biscuits
    Quantity: 3
    Price: 300

    Enter item name (leave blank to finish):

After this, the invoice is saved automatically.

------------------------------------------------------------------------

## 2. View Invoice by ID

Enter:

    Enter invoice ID: 1001

------------------------------------------------------------------------

## 🗂 3. Show All Invoices

Displays all invoice IDs stored in `index.txt`.

------------------------------------------------------------------------

##  Project Structure

    project/
       InvoiceGenerator.cpp
       index.txt
       invoices/
            invoice_1001.txt
            invoice_1002.txt

------------------------------------------------------------------------

## ✔ Adding Sample Data

Place sample invoices in the `invoices/` folder and update `index.txt`.

------------------------------------------------------------------------

##  You're Ready!

-   Create invoices\
-   View invoices\
-   Store invoices\
-   List all saved invoices
