## Notes
  * The project in this directory is a course from the link below. The code was provided, but I hava a decent understanding of it and can generalize to other data inputs and outputs.
    + link: https://spring.academy/courses/spring-cloud-stream
  * This directory contains a springboot multi module project application. It uses gradle as its depency manager.
  * The project mimics a producer-consumer application with an event-driven system and uses the spring-cloud-stream api for the abstraction need to work with "low-level framework code".
  * In this project, I used Apache kafka as the message broker(although others like RabbitMQ can be uses) and monitored the transactions between the produce-app and consumer-app.
  * Topics such as listing and processing http endpoints were covered. 

## End to End Integration Module(e2e-tests)
  * Although the end to end integration module(e2e-tests) is include, I do not know it as well as the other modules.

  ## How to run:
   * Use command below if using gradle. This is a gradle project so maven will not work. The command to build the project is:
     + ./gradlew bootRun