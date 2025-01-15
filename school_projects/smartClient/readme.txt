How to run:
    python SmartClient.py www.uvic.ca
Notes:
- If for some reason python does work try python3.
- On my home computer I was using python3, but python should work.
- Also can replace www.uvic.ca with some other website.
- Websites I tried and had no issues with are:
    www.google.com
    www.youtube.com
    www.facebook.com
    docs.engr.uvic.ca/docs
-In the parse_rsp() function uncommenting the comments that make calls 
to write to files will create a file(xyefcookies.txt) and write to it what the
client received after calling the socket.recv function.
    