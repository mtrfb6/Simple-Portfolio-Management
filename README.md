# Simple Portfolio Manager

This tool allows users to view stock data along with CAPM measures and create and edit theoretical portfolios accordingly. The data being used is not updated and thus, until this changes, this tool should only be taken as a proof of concept.


# Use/Functionality

Simple Portfolio Manager is a console application and has 3 main utilities. Viewing stock data, building and editing portfolios, and an informational section.

## View Stock Data - V

Users can view the list of available stocks by entering the **V** menu. Following this, if a user wishes, entering one of the tickers displayed above will bring up more detailed information, including: **sector, average return, variance, beta, and expected return**.

## Build Portfolio - B

The build menu, reached by entering **B** on the main menu screen, allows users to begin creating a portfolio. Entering the ticker symbol for the stock you wish to add will then prompt you with a quantity. If you wish to update or remove a quantity, entering the same ticker symbol will prompt you for the new amount.

## View Portfolio Breakdown - P

By entering **P** on the main menu, users can see a detailed breakdown of their portfolio, along with the CAPM measures provided for each stock and the portfolio itself.

## Information/Definitions - I

Since this program was designed to provide equitable access to more advanced financial measures, an informational section is provided to allow users to learn more about what each of the CAPM measures mean and some ideal values for certain cases.

# Data Sources:

All Stock data is gathered through the Yahoo Finance web scraping python script. Treasury Bill data was gathered in part via marketwatch. Each of these is read in from a .csv file containing the monthly closing price on each stock.
