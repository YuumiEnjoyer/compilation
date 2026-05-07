|           | <LETTER> | <DIGIT> |      .     | <оп> |   < >  |   SEP  |
|-----------|----------|---------|------------|------|--------|--------|
| START     |  ID      |  INT    |            |  OP  |  START |   SEP  |
| ID        |  ID      |  ID     |            |  END |  END   |   END  |
| INT       |          |  INT    |  FLOAT_DOT |  END |  END   |   END  |
| FLOAT_DOT |          |  FLOAT  |            |      |        |        |
| FLOAT     |          |  FLOAT  |            |  END |  END   |   END  |
| OP        |  END     |  END    |            |  END |  END   |   END  |
| SEP       |  END     |  END    |            |  END |  END   |   END  |
