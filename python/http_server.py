from fastapi import FastAPI, Request


app = FastAPI()

# pip install fastapi
# run command :uvicorn test:app --reload 
# 127.0.0.1:8000/api  

@app.post("/api")
def create_item(request: Request):
    print("recv msg")
    return {"code": 0, "msg": "success"}


@app.middleware("http")
async def TestCustomMiddleware(request: Request, call_next):
    the_headers = request.headers
    the_body = await request.json()

    print(the_headers)
    print(the_body)

    response = await call_next(request)

    return response