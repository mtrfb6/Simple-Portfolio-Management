import requests
from datetime import datetime, timedelta
import pandas as pd
from io import StringIO

#this is my modified data scraper that is way better and cooler and actually works unlike the other one

headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36'}
tickers = ["AAPL", "BEP", "COP", "CVX", "DIS", "FDX", "GOOGL", "JPM", "LEN", "LGIH", "LMT", "MAA", "MMM", "MSFT", "NFLX", "NKE", "NVDA", "PEP", "PFE", "PG", "PM", "PYPL", "SBUX", "SPY", "SQ", "TGT", "TMUS", "TSLA", "UL", "UNH", "V", "WM", "WMT", "XOM"]

#symbol = input("Symbol: ")
for i in tickers:
#while symbol != "exit":
    symbol = i
    days_back = 365 + 365 + 245
    now = datetime.utcnow()
    dateto = int(now.timestamp())
    dt = timedelta(days=days_back)
    datefrom = int((now - dt).timestamp())
    url = f'https://query1.finance.yahoo.com/v7/finance/download/{symbol}?period1={datefrom}&period2={dateto}&interval=1mo&filter=history&frequency=1mo&events=history&includeAdjustedClose=true'   
    r = requests.get(url, headers=headers)
    print(r)
    frame = pd.read_csv(StringIO(r.text), parse_dates=['Date'])
    frame.to_csv(f'../simple-portfolio-management/stockdata/{symbol}.csv')
    #symbol = input("Symbol: ")