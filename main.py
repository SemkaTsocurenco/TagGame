import matplotlib.pyplot as plt
import heapq
import time

BOARD_SIZE = 5  # размер поля 5x5

def board_to_tuple(board):
    """Преобразует список списков в неизменяемый кортеж кортежей (для хеширования состояния)."""
    return tuple(tuple(row) for row in board)

def get_piece_positions(state, piece):
    """Возвращает список координат, где расположена фишка с заданным номером."""
    positions = []
    for r in range(BOARD_SIZE):
        for c in range(BOARD_SIZE):
            if state[r][c] == piece:
                positions.append((r, c))
    return positions

def can_move_piece(state, piece, dr, dc):
    """
    Проверяет, можно ли переместить фишку с номером piece на (dr,dc).
    При перемещении все клетки фишки сдвигаются синхронно.
    Если новая позиция клетки не принадлежит самой фишке, то она должна быть пустой (0).
    """
    positions = get_piece_positions(state, piece)
    for (r, c) in positions:
        new_r, new_c = r + dr, c + dc
        # Проверка границ поля
        if new_r < 0 or new_r >= BOARD_SIZE or new_c < 0 or new_c >= BOARD_SIZE:
            return False
        # Если новая клетка не является текущей клеткой фишки, то она должна быть пустой
        if (new_r, new_c) not in positions:
            if state[new_r][new_c] != 0:
                return False
    return True

def move_piece(state, piece, dr, dc):
    """
    Возвращает новое состояние, полученное перемещением фишки piece на (dr,dc).
    Функция сначала очищает клетки, где находилась фишка, а затем записывает её в новые позиции.
    """
    new_board = [list(row) for row in state]
    positions = get_piece_positions(state, piece)
    # Освобождаем старые позиции
    for (r, c) in positions:
        new_board[r][c] = 0
    # Вычисляем новые позиции
    new_positions = [(r + dr, c + dc) for (r, c) in positions]
    for (r, c) in new_positions:
        new_board[r][c] = piece
    return board_to_tuple(new_board)

def get_all_moves(state):
    """
    Для данного состояния генерирует все допустимые ходы.
    Каждый ход – это перемещение некоторой фишки на одну клетку в одном из четырёх направлений.
    Возвращается список троек: (номер_фишки, направление, новое_состояние).
    """
    moves = []
    pieces = {cell for row in state for cell in row if cell != 0}
    directions = {
        'up': (-1, 0),
        'down': (1, 0),
        'left': (0, -1),
        'right': (0, 1)
    }
    for piece in pieces:
        for dname, (dr, dc) in directions.items():
            if can_move_piece(state, piece, dr, dc):
                new_state = move_piece(state, piece, dr, dc)
                moves.append((piece, dname, new_state))
    return moves

def get_top_left(state, piece):
    """
    Для фишки с номером piece возвращает координату (min_row, min_col)
    – левую верхнюю позицию, занятую этой фишкой.
    """
    positions = get_piece_positions(state, piece)
    if not positions:
        return None
    min_r = min(r for (r, _) in positions)
    min_c = min(c for (r, c) in positions if r == min_r)
    return (min_r, min_c)

def heuristic(state, target_top_left):
    """
    Эвристическая функция: сумма манхэттенских расстояний между левыми верхними координатами
    каждой фишки в текущем состоянии и в целевой конфигурации.
    """
    total = 0
    pieces = {cell for row in state for cell in row if cell != 0}
    for piece in pieces:
        current = get_top_left(state, piece)
        target = target_top_left[piece]
        total += abs(current[0] - target[0]) + abs(current[1] - target[1])
    return total

def a_star(initial, target_top_left):
    """
    Поиск A* для нахождения последовательности ходов от начального состояния до целевого.
    Возвращает список ходов, где каждый ход – кортеж (номер_фишки, направление, новое_состояние).
    """
    open_set = []
    initial_h = heuristic(initial, target_top_left)
    heapq.heappush(open_set, (initial_h, 0, initial, []))
    visited = {initial: 0}
    while open_set:
        f, g, state, path = heapq.heappop(open_set)
        if state == target_state:
            return path
        for (piece, dname, new_state) in get_all_moves(state):
            new_g = g + 1
            if new_state not in visited or new_g < visited[new_state]:
                visited[new_state] = new_g
                new_path = path + [(piece, dname, new_state)]
                new_f = new_g + heuristic(new_state, target_top_left)
                heapq.heappush(open_set, (new_f, new_g, new_state, new_path))
    return None

def print_board(state):
    """Красивый вывод состояния поля в консоли."""
    print("+-----" * BOARD_SIZE + "+")
    for row in state:
        row_str = ""
        for cell in row:
            if cell == 0:
                cell_str = "     "
            else:
                cell_str = f"  {cell}  "
            row_str += "|" + cell_str
        row_str += "|"
        print(row_str)
        print("+-----" * BOARD_SIZE + "+")

def visualize_solution(solution, delay=1.0):
    """
    Визуализирует последовательность состояний (решение) с помощью matplotlib.
    Для каждого шага показывается отдельное окно (с обновлением графика).
    """
    plt.ion()  # включаем интерактивный режим
    fig, ax = plt.subplots()
    for i, state in enumerate(solution):
        ax.clear()
        # Формируем числовой массив для отображения
        data = [[cell for cell in row] for row in state]
        ax.imshow(data, cmap='Pastel1', vmin=-1, vmax=10)
        # Добавляем текстовую аннотацию в каждую клетку
        for r in range(BOARD_SIZE):
            for c in range(BOARD_SIZE):
                cell = state[r][c]
                text = " " if cell == 0 else str(cell)
                ax.text(c, r, text, ha='center', va='center', fontsize=16, fontweight='bold')
        ax.set_title(f"Шаг {i}")
        ax.set_xticks([])
        ax.set_yticks([])
        plt.draw()
        plt.pause(delay)
    plt.ioff()
    plt.show()

if __name__ == "__main__":
    # Исходное состояние (0 – пустая клетка)
    initial_board = [
        [1, 1, 1, 2, 2],
        [3, 3, 4, 4, 2],
        [5, 6, 6, 7, 7],
        [5, 5, 8, 8, 8],
        [9, 0, 0, 0, 10]
    ]
    initial_state = board_to_tuple(initial_board)
    
    # Целевая конфигурация:
    # В правом верхнем углу (ячейки [0][2]..[1][4]) формируется прямоугольник 3×2, состоящий из двух трёхклеточных фишек:
    # Фишка 5 занимает ячейки (0,2), (1,2) и (1,3)
    # Фишка 2 занимает ячейки (0,3), (0,4) и (1,4)
    # Остальные фишки размещены так, чтобы сохранить свои исходные формы.
    # target_board = [
    #     [6, 6, 5, 2, 2],
    #     [7, 7, 5, 5, 2],
    #     [1, 1, 1, 3, 3],
    #     [4, 4, 8, 8, 8],
    #     [9, 0, 0, 0, 10]
    # ]


    target_board = [
        [3, 3, 1, 1, 1],
        [9, 4, 4, 6, 6],
        [5, 10, 2, 2, 0],
        [5, 5, 0, 2, 0],
        [8, 8, 8, 7, 7]
    ]
    target_state = board_to_tuple(target_board)
    
    # Для каждой фишки вычисляем её «левую верхнюю» координату в целевом состоянии (используется в эвристике)
    target_top_left = {}
    pieces = {cell for row in target_state for cell in row if cell != 0}
    for piece in pieces:
        target_top_left[piece] = get_top_left(target_state, piece)
    
    print("Исходное состояние:")
    print_board(initial_state)
    print("\nЦелевая конфигурация:")
    print_board(target_state)
    
    print("\nПоиск решения (A*)...")
    solution_moves = a_star(initial_state, target_top_left)
    if solution_moves is None:
        print("Решение не найдено.")
    else:
        print(f"Решение найдено за {len(solution_moves)} ходов.")
        for i, (piece, dname, _) in enumerate(solution_moves, start=1):
            print(f"Ход {i}: переместить фишку {piece} {dname}")
        
        # Формируем последовательность состояний для визуализации:
        states_sequence = [initial_state] + [move[2] for move in solution_moves]
        print("\nВизуализация решения...")
        visualize_solution(states_sequence, delay=1.0)
