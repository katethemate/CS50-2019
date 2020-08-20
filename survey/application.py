import cs50
import csv

from flask import Flask, jsonify, redirect, render_template, request

# Configure application
app = Flask(__name__)

# Reload templates when they are changed
app.config["TEMPLATES_AUTO_RELOAD"] = True


@app.after_request
def after_request(response):
    """Disable caching"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/", methods=["GET"])
def get_index():
    return redirect("/form")


@app.route("/form", methods=["GET"])
def get_form():
    return render_template("form.html")


@app.route("/form", methods=["POST"])
def post_form():
    if not request.form.get('name') or request.form.get('selecthouse')=='House':
        return render_template("error.html", message="Pls fill out the survey completely")
    try:
        with open('survey.csv', 'a', newline='') as csvfile:
            fieldnames = ['name', 'house', 'role']
            writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
            writer.writerow({'name': request.form.get("name"), 'house': request.form.get("selecthouse"), 'role': request.form.get("customRadio")})
    except IOError:
        sys.exit(f"Could not read survey.csv")

    return redirect("/sheet")

@app.route("/sheet", methods=["GET"])
def get_sheet():
    with open('survey.csv', 'r') as file:
        reader = csv.DictReader(file)
        students = list(reader)
    return render_template("table.html", students=students)