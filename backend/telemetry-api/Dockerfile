#Stage 1
FROM eclipse-temurin:21-jdk AS build
WORKDIR /app
COPY .mvn / .mvn/
COPY mvnw pom.xml ./
RUN chmod +x mvnw #security clearance
COPY src ./src
RUN ./mvnw clean package -DskipTests

#Stage 2 rt

FROM eclipse-temurin:21-jre
WORKDIR /app
COPY --from=build /app/target/*.jar app.jar
EXPOSE 8080
ENTRYPOINT ["java", "-jar", "app.jar"]

