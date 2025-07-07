DROP TABLE IF EXISTS iris;
CREATE TABLE iris(
  id      SERIAL PRIMARY KEY,
  sepal_l FLOAT,
  sepal_w FLOAT,
  petal_l FLOAT,
  petal_w FLOAT,
  specie  VARCHAR(20)
);