from flask import Flask

app = Flask(__name__)

print('Starting flask server')

@app.route('/')
def hello_world():
    print('Sending Hello World')
    return 'Hello World!'

@app.route('/<name>')
def hello_name(name):
    print('Sending Hello', name)
    return 'Hello {}!'.format(name)


# print('Hello World')
