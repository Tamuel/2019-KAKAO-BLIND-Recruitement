# 2019-KAKAO-BLIND-test
My source code for 2019 KAKAO BLIND test.

개인적으로 풀어본 2019 KAKAO 블라인드 1차, 2 테스트 C++ 소스코드입니다.

많은 분들이 참고하셨으면 좋겠고 혹시 문의 사항이나, 제가 틀린 곳이 있다면 Issues나 이메일 주시면 바로 답장 드리겠습니다.

---

* 1차 테스트 주소 : http://tech.kakao.com/2018/09/21/kakao-blind-recruitment-for2019-round-1/

* 2차 테스트 주소 : http://tech.kakao.com/2018/10/23/kakao-blind-recruitment-round-2/
  * 2차 테스트 Github 주소 : https://github.com/kakao-recruit/2019-blind-2nd-elevator

---
## 2차 테스트 결과
![엘리베이터 데모](https://github.com/Tamuel/2019-KAKAO-BLIND-test/blob/master/2%EC%B0%A8/2nd.gif)

* **문제 0 (엘리베이터 4개, 수용 8명)**
  * 층 : 5층
  * Call : 6개

    |AveWait|AveTravel|AVeTotal|LastTs|Status|
    |-------|---------|--------|------|------|
    |2.333|7.333|9.666|16|OK|

* **문제 1 (엘리베이터 4개, 수용 8명)**
  * 층 : 25층
  * Call : 200개

    |AveWait|AveTravel|AVeTotal|LastTs|Status|
    |-------|---------|--------|------|------|
    |25.050|23.095|48.145|592|OK|
  
* **문제 2 (엘리베이터 4개, 수용 8명)**
  * 층 : 25층
  * Call : 500개
  * 1층 출입구, 1 13층간 이동 많음

    |AveWait|AveTravel|AVeTotal|LastTs|Status|
    |-------|---------|--------|------|------|
    |12.108|17.232|29.340|1868|OK|
  
---

* 이메일 : eastern7star@gmail.com
