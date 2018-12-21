Some things to test further:

* Using zhelpers.h at all? <- Nope.
* What about large messages? <- I'm just doing this wrong. I need to swap zmq_recv / send with zmq_msg* variants
* Can the SB interface be replicated well enough? Subscribe, send, receive?
* Push some stuff into headers?
