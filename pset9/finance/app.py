# API_KEY=pk_b667c826945741a48239eda25ed474ee

import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash
from datetime import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Query database for user's purchases
    purchases = db.execute(
        "SELECT symbol, name, SUM(shares) as sharesSum FROM purchases WHERE user_id = ? GROUP BY symbol", session["user_id"])

    # Change the price to current price
    stockTotal = 0
    purchasesBySymbol = []
    for purchase in purchases:
        if purchase["sharesSum"] > 0:
            price = lookup(purchase["symbol"])["price"]
            total = purchase["sharesSum"] * price
            stockTotal += total
            purchasesBySymbol.append({
                "symbol": purchase["symbol"],
                "name": purchase["name"],
                "shares": purchase["sharesSum"],
                "price": price,
                "total": total
            })

    # Query for cash
    cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]

    # Total money
    total = cash + stockTotal

    return render_template("index.html", purchases=purchasesBySymbol, cash=cash, total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol is not blank and exists
        if not request.form.get("symbol") or not lookup(request.form.get("symbol")):
            return apology("Invalid symbol")

        # Ensure share is a positive and is an integer
        if not request.form.get("shares").isdigit() or float(request.form.get("shares")) <= 0.0:
            return apology("Please enter a positive integer")

        # Get the valid values
        symbol = request.form.get("symbol")
        name = lookup(symbol)["name"]
        shares = int(request.form.get("shares"))
        price = lookup(symbol)["price"]
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        userCash = float(rows[0]["cash"])

        # Check uesr has enough money
        if userCash < shares * price:
            return apology("CAN'T AFFORD")

        # Update user's cash after transaction
        db.execute("UPDATE users SET cash = ? WHERE id = ?", userCash - shares * price, session["user_id"])

        # Store the transaction's (symbol, share, price, date) info into database.
        date = datetime.now().strftime("%Y/%m/%d %H:%M:%S")
        db.execute("INSERT INTO purchases (user_id, symbol, name, shares, price, date) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol, name, shares, price, date)

        # flash bought alert
        flash("Stock successfully bought!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Query database for purchases
    purchases = db.execute("SELECT symbol, name, shares, price, date FROM purchases WHERE user_id = ?", session["user_id"])

    return render_template("history.html", purchases=purchases)


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
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

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

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol is not blank and exists
        if not request.form.get("symbol") or not lookup(request.form.get("symbol")):
            return apology("Invalid symbol")

        # Render quoted.html with the returned quote values
        quote = lookup(request.form.get("symbol"))
        return render_template("quoted.html", quote=quote)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username is not blank
        if not username:
            return apology("must provide username")

        # Ensure username does not already exist
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        if len(rows) > 0:
            return apology("username already exists")

        # Ensure password & confirmation are not blank
        if not password or not confirmation:
            return apology("must provide password and confirmation")

        # Ensure password match confirmation
        if password != confirmation:
            return apology("passwords do not match")

        # Insert new user's (username, hash) into database
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", username, generate_password_hash(password))

        # Automatically log the user in after successfully registered
        rows = db.execute("SELECT * FROM users WHERE username = ?", username)
        session["user_id"] = rows[0]["id"]

        # Flash successful register message
        flash("Successfully registered !")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get symbol
        symbol = request.form.get("symbol")

        # Ensure stock is selected
        if not symbol:
            return apology("Invalid symbol")

        # Query for total shares for selected symbol
        rows = db.execute("SELECT SUM(shares) AS totalShares FROM purchases WHERE user_id = ? AND symbol = ?",
                          session["user_id"], symbol)

        # Ensure user own shares of the selected stock
        if len(rows) != 1:
            return apology("You does NOT own any share of this stock")

        # Get shares to sell and shares owned
        sharesToSell = request.form.get("shares")
        sharesOwned = rows[0]["totalShares"]

        # Ensure sharesToSell is a positive integer
        if float(sharesToSell) <= 0.0 or not sharesToSell.isdigit():
            return apology("Please enter a positive integer")

        # Ensure user own enough shares to sell
        sharesToSell = int(sharesToSell)
        if sharesToSell > sharesOwned:
            return apology("Not enough shares to sell")

        # Store the transaction in the purchases database table
        name = lookup(symbol)["name"]
        price = lookup(symbol)["price"]
        date = datetime.now().strftime("%Y/%m/%d %H:%M:%S")
        db.execute("INSERT INTO purchases (user_id, symbol, name, shares, price, date) VALUES (?, ?, ?, ?, ?, ?)",
                   session["user_id"], symbol, name, -sharesToSell, price, date)

        # Add sold stock to cash
        cash = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])[0]["cash"]
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + sharesToSell * price, session["user_id"])

        # flash successfully sold alert
        flash("Stock successfully sold!")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        rows = db.execute("SELECT DISTINCT symbol FROM purchases WHERE user_id = ?", session["user_id"])
        return render_template("sell.html", rowsOfSymbols=rows)


@app.route("/addCash", methods=["GET", "POST"])
@login_required
def addCash():
    """Add additional Cash to user's account"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get cash amount
        cashToAdd = request.form.get("cash")

        # Ensure cash is positive
        if not cashToAdd or float(cashToAdd) < 0:
            return apology("Invalid cash amount", 403)

        # Cast cashToAdd to float
        cashToAdd = float(cashToAdd)

        # Query for current cash
        rows = db.execute("SELECT cash FROM users WHERE id = ?", session["user_id"])
        cash = rows[0]["cash"]

        # Add cash to user's cash
        db.execute("UPDATE users SET cash = ? WHERE id = ?", cash + cashToAdd, session["user_id"])

        # flash cash successfully added alert
        flash("Cash successfully added")
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("addCash.html")
