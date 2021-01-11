import os

import sqlite3
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from re import compile, search

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure dict_factory for SQLite database
def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d

# Configure SQLite database
con = sqlite3.connect("finance.db", check_same_thread=False)
con.row_factory = dict_factory
cur = con.cursor()

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Retrieve user data
    stocks = cur.execute("""SELECT name, wallet_symbol, wallet_shares
                            FROM wallet
                            WHERE user_wallet = ?
                            ORDER BY wallet_symbol""", (session["user_id"],)).fetchall()
    current_cash = cur.execute("SELECT cash FROM users WHERE id = ?", (session["user_id"],)).fetchone()

    # Retrieve total value of user holding
    total_holding = current_cash["cash"]
    current_cash = current_cash["cash"]
    
    # If transaction(s) had been made, compute current value of Portfolio holdings
    if stocks:
        for stock in stocks:
            
            # Retrieve current stock data
            updated_stock_data = lookup(stock["wallet_symbol"])
            
            # Update price value (per stocks, per units)
            stock["price"] = updated_stock_data["price"]

            # Update total value
            stock["total"] = stock["price"] * stock["wallet_shares"]
            total_holding += stock["total"]

    # Return template with user data
    return render_template("index.html", stocks=stocks, total_holding=total_holding, current_cash=current_cash)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # If user submit buy form
    if request.method == "POST":
        
        """Verify Usage"""

        # Retrieve user input
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Verify usage
        quote = lookup(symbol)
        if not quote or int(shares) <= 0:
            return render_template("buy.html", invalid_input=True)

        # Query database for user cash
        user_cash = cur.execute("SELECT cash FROM users WHERE id = ?", (session["user_id"],)).fetchone()
        
        # Verify user has enough funds
        total_price = quote["price"] * int(shares)
        remaining_cash = user_cash["cash"] - total_price
        if remaining_cash < 0:
            return render_template("buy.html", no_funds=True)


        """Verification passed, insert / update on wallet"""

        # Adjust variables to fit correctly
        symbol = symbol.upper()
        shares = int(shares)

        # Verify if user already has stocks from this company on his wallet
        wallet_shares = cur.execute("""SELECT wallet_shares
                                        FROM wallet
                                        WHERE user_wallet = ?
                                        AND wallet_symbol = ?""", (session["user_id"], symbol,)).fetchone()

        # If user doesn't have shares from this company, insert data to wallet
        if not wallet_shares:
            wallet_insert = (session["user_id"], quote["name"], symbol, shares,)
            cur.execute("""INSERT INTO wallet
                            (user_wallet, name, wallet_symbol, wallet_shares)
                            VALUES (?, ?, ?, ?)""", wallet_insert)

        # User already have shares from this company on his wallet, update data
        else:
            updated_shares = wallet_shares["wallet_shares"] + shares
            wallet_update = (updated_shares, session["user_id"], symbol)
            cur.execute("""UPDATE wallet
                            SET wallet_shares = ?
                            WHERE user_wallet = ?
                            AND wallet_symbol = ?""", wallet_update)


        """Insert transaction in history and update funds"""

        # Init
        cash = (remaining_cash, session["user_id"],)
        history = (session["user_id"], symbol, shares, quote["price"],)
        
        cur.execute("""INSERT INTO history
                        (user_history, history_symbol, history_shares, price, timestamp)
                        VALUES (?, ?, ?, ?, datetime())""", history)
        cur.execute("UPDATE users SET cash = ? WHERE id = ?", cash)
        
        # Transaction over, commit and redirect
        con.commit()
        return redirect("/")

    # User reached route via GET (as by clicking a link)
    return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Retrieve history data
    history = cur.execute("""SELECT history_symbol, history_shares, price, timestamp
                            FROM history
                            WHERE user_history = ?
                            ORDER BY timestamp DESC""", (session["user_id"],)).fetchall()
    
    # Return template with history
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        username_input = (request.form.get("username"),)
        user_data = cur.execute("SELECT * FROM users WHERE username = ?",
                          username_input).fetchall()

        # Ensure username exists and password is correct
        if len(user_data) != 1 or not check_password_hash(user_data[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = user_data[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # If user submit quote form
    if request.method == "POST":
        
        # Retrieve symbol data
        quote = lookup(request.form.get("symbol"))

        # Verify Usage
        if quote is None:
            return render_template("quote.html", invalid=True)

        # Return values requested
        return render_template("quote.html", success=True, quote=quote)

    # User reached route via GET (by clicking a link)
    return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # If user submit registration
    if request.method == "POST":

        """Verify usage"""

        # Retrieve user's input
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Verify usage
        if not username:
            return apology("must provide username", 403)
        elif not password or not confirmation:
            return apology("must provide password", 403)
        elif password != confirmation:
            return apology("passwords does not match", 403)

        # Verify password strength via regex
        regex = "^(?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*#?&])[A-Za-z\d@$!#%*?&]{8,20}$"
        re_compiled = compile(regex)
        match = search(re_compiled, password)
        if not match:
            part_1 = "Password must contain one uppercase and one lowercase letter"
            part_2 = ", one digit, one special symbol (@$!#%*?&)"
            part_3 = ", and be between 8 to 20 characters long"
            return apology(part_1 + part_2 + part_3, 403)
        
        # Verify is username is not already taken
        cur.execute("SELECT * FROM users WHERE username = ?", (username,))
        if cur.fetchone():
            return apology("username " + username + " is already taken", 403)

        """Verification passed, create account"""

        password_hash = generate_password_hash(password)
        cur.execute("INSERT INTO users (username, hash) VALUES (?, ?)", (username, password_hash,))
        con.commit()

        # Remember which user has logged in, redirect to index
        session["user_id"] = cur.lastrowid
        return redirect("/")

    # User reached route via GET (by clicking a link)
    return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # If user submit sell form
    if request.method == "POST":
        
        """Verify usage"""

        # Retrieve users input
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Verify user entered a positive amount of shares
        if shares <= 0:
            return apology("enter a positive amount of shares")

        # Retrieve user data
        user_data = cur.execute("""SELECT wallet_shares, cash
                            FROM wallet
                            INNER JOIN users
                            ON wallet.user_wallet = users.id
                            WHERE wallet_symbol = ?
                            AND user_wallet = ?""", (symbol, session["user_id"],)).fetchone()

        # Verify user has enough shares from this company
        if shares > user_data["wallet_shares"]:
            return apology("not enough shares")
        
        """Verification passed, sell shares, update database"""

        # Init updated values
        symbol_data = lookup(symbol)
        new_funds = user_data["cash"] + symbol_data["price"] * shares
        wallet_shares = user_data["wallet_shares"] - shares
        history_shares = shares - shares - shares
        
        # Delete record from wallet if user has no shares left on this company
        if not wallet_shares:
            cur.execute("""DELETE FROM wallet
                            WHERE wallet_symbol = ?
                            AND user_wallet = ?""", (symbol, session["user_id"],))
        
        # If user still has shares on this company on his wallet, update
        else:
            cur.execute("""UPDATE wallet
                            SET wallet_shares = ?
                            WHERE wallet_symbol = ?
                            AND user_wallet = ?""", (wallet_shares, symbol, session["user_id"],))

        # Update user cash
        cur.execute("UPDATE users SET cash = ? WHERE id = ?", (new_funds, session["user_id"],))

        # Insert transaction into history
        history = (session["user_id"], symbol, history_shares, symbol_data["price"],)
        cur.execute("""INSERT INTO history
                        (user_history, history_symbol, history_shares, price, timestamp)
                        VALUES (?, ?, ?, ?, datetime())""", history)

        # Commit changes and redirect
        con.commit()

        return redirect("/")


    # User reached route via GET (by clicking a link)
    # Retrieve user symbols and render template with 
    symbols = cur.execute("""SELECT wallet_symbol
                            FROM wallet
                            WHERE user_wallet = ?""",
                            (session["user_id"],)).fetchall()

    return render_template("sell.html", symbols=symbols)



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
