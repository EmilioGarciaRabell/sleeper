# Sleeper
Have you ever had an amazing idea at night and completely forgot about it? The Dreamer can help you log these ideas, no need to go on your phone, press a button and talk, you will have a transcribed version of your thoughts on a comprehensive website.


# Webapp Setup

## 1. Configure the Backend (Flask)

Navigate into your `webapp/back directory`. We will set up a virtual environment to keep your Python dependencies isolated.

```
cd webapp/back
python -m venv .venv
# Activate it:
source .venv/bin/activate  # On Windows: .venv\Scripts\activate
```
## 2. Install Dependencies:
```
pip install flask flask-cors
```

## 3. Run `app.py`

```
python app.py
```
## 4. Front end setup   

Navigate to `webapp/front`. You need to install the project dependencies defined in your `package.json`, and run the front end.

```
cd ../front
npm install
npm run dev
```


