import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

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

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():

    users = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])
    stocks = db.execute(
        "SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares > 0", user_id=session["user_id"])
    quotes = {}

    for stock in stocks:
        quotes[stock["symbol"]] = lookup(stock["symbol"])

    cash_remaining = users[0]["cash"]

    total = cash_remaining

    for quote in quotes.values():
        total += quote["price"]

    return render_template("index.html", quotes=quotes, stocks=stocks, total=total, cash_remaining=cash_remaining)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        #Get stock information
        quote = lookup(request.form.get("symbol"))

        #Validate ticker symbol
        if quote == None:
            return apology("invalid ticker symbol")

        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("must be a number")

        if shares<0:
            return apology("needs to be a positive number")

        rows = db.execute("SELECT cash FROM users WHERE id = :user_id",
                            user_id=session["user_id"])

        available_cash = rows[0]["cash"]

        total_price = shares * quote["price"]

        if available_cash < total_price:
            return apology("insufficient funds")



        db.execute("UPDATE users SET cash = cash - :total_price WHERE id = :user_id", total_price=total_price, user_id = session["user_id"])
        db.execute("INSERT INTO 'transactions' (user_id, symbol, shares, price, action) VALUES (:user_id, :symbol, :shares, :price, :action)", user_id=session["user_id"], symbol=request.form.get("symbol"), shares=shares, price=quote["price"], action="bought")
        stocks = db.execute("SELECT cash, symbol, shares, price FROM transactions INNER JOIN users ON transactions.user_id = users.id WHERE transactions.user_id= :user_id", user_id=session["user_id"])
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""


    username = request.args.get('username')
    print(username)
    taken = db.execute("SELECT username FROM users")

    usernames = []
    for name in taken:
        usernames.append(name['username'])

    if len(username)>0 and username not in usernames:
        return jsonify(True)
    else:
        return jsonify(False)



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    transactions = db.execute(
        "SELECT symbol, price, action, time, shares FROM transactions WHERE user_id = :user_id ORDER BY time ASC", user_id=session["user_id"])

    return render_template("history.html", transactions = transactions)


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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

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
    if request.method == "POST":

        #Get stock information
        stock = lookup(request.form.get("symbol"))
        print(stock)
        #Validate ticker symbol
        if stock == None:
            return apology("invalid ticker symbol")

        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""


        #User reached route via POST
    if request.method == "POST":


        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        elif not request.form.get("confirmation"):
            return apology("must confirm your password", 403)


        #Hash the password
        hash = generate_password_hash(request.form.get("password"))

        #Insert user into database
        result = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                            username=request.form.get("username"), hash=hash)

        if not result:
            return apology("username already exists")
        else:
            return apology("registration complete", 200)


        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    #User reached route via GET
    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "POST":

        quote = lookup(request.form.get("symbol"))

        # Check if the symbol exists
        if quote == None:
            return apology("invalid symbol", 400)

        # Check if shares was a positive integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be a positive integer", 400)

        # Check if # of shares requested was 0
        if shares <= 0:
            return apology("can't sell less than or 0 shares", 400)

        # Check if we have enough shares
        stock = db.execute("SELECT SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id AND symbol = :symbol GROUP BY symbol",
                           user_id=session["user_id"], symbol=request.form.get("symbol"))

        if len(stock) != 1 or stock[0]["total_shares"] <= 0 or stock[0]["total_shares"] < shares:
            return apology("you can't sell less than 0 or more than you own", 400)

        # Query database for username
        rows = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])

        # How much $$$ the user still has in her account
        cash_remaining = rows[0]["cash"]
        price_per_share = quote["price"]

        # Calculate the price of requested shares
        total_price = price_per_share * shares

        # Book keeping (TODO: should be wrapped with a transaction)
        db.execute("UPDATE users SET cash = cash + :price WHERE id = :user_id", price=total_price, user_id=session["user_id"])
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price, action) VALUES(:user_id, :symbol, :shares, :price, :action)",
                   user_id=session["user_id"],
                   symbol=request.form.get("symbol"),
                   shares=-shares,
                   price=price_per_share, action="sold")


        return redirect("/")

    else:


        stocks = db.execute("SELECT symbol, SUM(shares) as total_shares FROM transactions WHERE user_id = :user_id GROUP BY symbol HAVING total_shares>0", user_id=session["user_id"])
        return render_template("sell.html", stocks=stocks)

@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    if request.method == "POST":

        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        confirmation =request.form.get("confirmation")
        current_hash = db.execute("SELECT hash FROM users WHERE id = :user_id", user_id=session["user_id"])

        print(current_hash[0]['hash'], generate_password_hash(old_password))


        if new_password == confirmation and old_password != new_password and check_password_hash(current_hash[0]['hash'], old_password):
            new_hash = generate_password_hash(new_password)
            db.execute("UPDATE users SET hash = :new_hash WHERE id = :user_id", new_hash=new_hash, user_id=session["user_id"])

            flash("Changed!")
            return apology("password changed", 200)

        else:
            return apology("oops check again!", 400)

    #User reached route via GET
    else:
        return render_template("change_password.html")



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
