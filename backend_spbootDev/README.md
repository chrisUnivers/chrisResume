## Notes
  * The project in this directory is a course from the link below. The code was provided, but I hava a decent understanding of it and can generalize to other data inputs and outputs.
    + link: https://spring.academy/courses/spring-cloud-stream
  * This directory contains a springboot multi module project application. It uses gradle as its depency manager.
  * The project mimics a publisher-consumer application with an event-driven system and uses the spring-cloud-stream framework with its's APIs for the abstraction needed to work with "low-level framework code". Connecting to middleware, publishing and consuming and more.
  * The purpose of the application is to track and manage activities a Cash Card system. Basically creating a cash card, when it was used, how much was used, who used the cash card and more.
    + The application consist of three Spring Cloud Stream microservice applications: a source, a processor and sink. All three contain their own tests. 
    + The main activities can be found in the `transaction-domain and transaction-source folder`. 
    + No databases were used so the transactions were produced by the Source application.
    + The transactions are then consumed by the Processor
  * The three applications:
    + **The source**(ues the Java function Supplier Interface): 
      1. produces fake transactions and sends them to a kafka topic.
      2. Handles HTTP POST request using a REST controller endpoint. 
    +  **The Processor**(using java Function Interface): 
      1. consumes the transactions. 
      2. creates a new transaction with additional info. Card holder info and whether approved or not
      3. published to new topic.
    + **The sink**(Using java Consumer Interface): 
      1. consumes the bindings from the processor
      2. writes it to a cvs file.
  * In this project, I used Apache kafka as the message middleware(although others like RabbitMQ can be uses) and monitored the transactions between the produce-app and consumer-app. Docker was used to run kafka.
    + To facilitate the communication between kafka and the application the Spring Cloud Stream Kafka Binder dependency was used.
    + Kafka was used to verify that the bindings(transaction) were actually going through the middleware.
    + The commands used to interact with kafka through the docker were provided.
  * Testing was also covered and the Spring cloud stream test bind was used for this. 

## End to End Integration Module(e2e-tests)
  * Although the end to end integration module(e2e-tests) is include, I don't hava a general understanding of it like the other three: source, processor, sink.

  ## How to run:
   * Use command below if using gradle. This is a gradle project so maven will not work. The commands to three three app in are:
     + ./gradlew cashcard-transaction-source:bootRun
     + ./gradlew cashcard-transaction-enricher:bootRun --args="--spring.cloud.stream.bindings.enrichTransaction-in-0.destination=approvalRequest-out-0"
     + ./gradlew cashcard-transaction-sink:bootRun --args="--spring.cloud.stream.bindings.sinkToConsole-in-0.destination=enrichTransaction-out-0"
  * Use the command below to run the tests.
    + ./gradlew test