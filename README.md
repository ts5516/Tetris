# Tetris
## 테트리스 게임



### 테트리스 키 사용법

← : 테트로미노 왼쪽으로 1칸 이동  
→ : 테트로미노 오른쪽으로 1칸 이동  
↑ : 테트로미노 시계 방향으로 90도 회전  
spaceBar : hard drop   
esc : 게임 일시 정지(시간은 정상 작동함)

###테트리스 AI
1. 각 상황에 따라 가중치 연산
2. 경우의 수 중에서 유망한 해를 결정
3. 목적지를 향해 0.2초 간격으로 회전 및 이동
4. 목적지에 도달하면 Hard drop발생
