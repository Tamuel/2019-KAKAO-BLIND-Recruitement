import requests
from collections import OrderedDict

url = 'http://localhost:8000'


def start(user, problem, count):
    uri = url + '/start' + '/' + user + '/' + str(problem) + '/' + str(count)
    return requests.post(uri).json()


def oncalls(token):
    uri = url + '/oncalls'
    return requests.get(uri, headers={'X-Auth-Token': token}).json()


def action(token, cmds):
    uri = url + '/action'
    return requests.post(uri, headers={'X-Auth-Token': token}, json={'commands': cmds}).json()


class Elevator:
    def __init__(self, _id):
        self.id = _id
        self.floor = 1
        self.destination = dict()
        self.final_destination = 1
        self.passenger = dict()  # Assigned passenger
        self.passenger_possess = dict()
        self.status = 'STOPPED'
        self.direction = 0  # 1 : up, -1 : down, 0 : stop

    def insert(self, dst):
        self.destination[dst] = self.destination.get(dst, 0) + 1
        self.destination = dict(sorted(self.destination.items(), key=lambda kv : kv[0]))

    def remove(self, dst):
        if dst in self.destination:
            self.destination.pop(dst)

    def set_final(self, dst):
        self.insert(dst)
        self.final_destination = dst
        self.direction = 1 if dst > self.floor else (-1 if dst < self.floor else 0)

    def need_to_stop(self, dst):
        return dst in self.destination

    def need_to_exit(self, dst):
        exit_passenger_ids = []
        for p in self.passenger_possess:
            if self.passenger_possess[p].end == dst:
                exit_passenger_ids.append(self.passenger_possess[p].id)

        return exit_passenger_ids

    def erase_passengers(self, passenger_ids):
        for p in passenger_ids:
            self.passenger.pop(p)
            self.passenger_possess.pop(p)

    def erase_destination(self, dst):
        self.destination.pop(dst)

    def enter_passenger(self, passenger_ids):
        for p_i in passenger_ids:
            self.passenger_possess[p_i] = self.passenger[p_i]
            self.insert(self.passenger[p_i].end)

    def passenger_check(self, passenger_ids):
        passengers = []
        for p_i in passenger_ids:
            if p_i in self.passenger:
                passengers.append(p_i)

        return passengers


class Passenger:
    def __init__(self, _id, _start, _end, _timestamp):
        self.id = _id
        self.start = _start
        self.end = _end
        self.timestamp = _timestamp
        # 1 : up, -1 : down, 0 : stop
        self.dir = 1 if _end > _start else (-1 if _end < _start else 0)


def p0_solution():
    user = 'tester'
    problem = 2
    count = 4

    ret = start(user, problem, count)
    token = ret['token']
    print('Token for %s is %s' % (user, token))

    elevators_json = ret['elevators']

    # Make elevator object
    elevators = {}
    for i in elevators_json:
        elevators[i['id']] = Elevator(i['id'])

    # Make passengers map
    passengers = OrderedDict()

    elev_max = 8
    n_floor = 25 + 1
    floor_waiting = []  # 배정받은 엘리베이터 없이 기다리는 승객
    floor_exist = []  # 승객 현 위치

    for i in range(n_floor):
        floor_waiting.append(OrderedDict())
        floor_exist.append(OrderedDict())

    while True:
        on = oncalls(token)
        if on['is_end']:
            break
        calls = on['calls']
        elev = on['elevators']

        for c in calls:
            if not c['id'] in passengers:
                passengers[c['id']] = Passenger(c['id'], c['start'], c['end'], c['timestamp'])
                floor_exist[c['start']][c['id']] = c['id']
                floor_waiting[c['start']][c['id']] = c['id']

        for e in elev:
            elevators[e['id']].floor = e['floor']
            elevators[e['id']].status = e['status']

        print("T:", on['timestamp'], token)

        # for p in passengers:
        #     print("P-ID:", passengers[p].id, ", START:", passengers[p].start, ", END:", passengers[p].end,
        #           ", TIMESTAMP:", passengers[p].timestamp)

        for e in elevators:
            print("E-ID:", elevators[e].id, ", FLOOR:", elevators[e].floor, ", Final DST:", elevators[e].final_destination,
                  ", DST:", list(elevators[e].destination.keys()), ", PASSENGERS:", list(elevators[e].passenger.keys()),
                  ", PAS_POSSESS:", list(elevators[e].passenger_possess.keys()))

        # Find where to go
        for e in elevators:
            e = elevators[e]
            if e.status == 'STOPPED' or (e.status == 'OPENED' and e.final_destination == e.floor):
                # 멈춰있고 태울 승객이 없는 경우 기다리는 승객이 가장 많은 곳으로
                if len(e.passenger) == 0:
                    max_waiting_floor = e.floor
                    max_temp = 0
                    # 먼 승객 우선
                    for i in range(e.floor, n_floor):
                        if len(floor_waiting) != 0 and len(floor_waiting[i]) >= max_temp:
                            max_temp = len(floor_waiting[i])
                            max_waiting_floor = i
                    for i in range(e.floor, 0, -1):
                        if len(floor_waiting) != 0 and len(floor_waiting[i]) >= max_temp:
                            max_temp = len(floor_waiting[i])
                            max_waiting_floor = i

                    _set = False
                    while len(floor_waiting[max_waiting_floor]) > 0 and len(e.passenger) < elev_max:
                        _set = True
                        p_id = floor_waiting[max_waiting_floor].pop(list(floor_waiting[max_waiting_floor].items())[0][0])
                        e.passenger[p_id] = passengers[p_id]
                    if _set:
                        e.set_final(max_waiting_floor)
                elif e.floor == e.final_destination:  # 우선 순위 목적지에 도달한 경우
                    # 기다리는 승객이 있으면 받음
                    for f in range(e.floor, n_floor):
                        will_del = []
                        for p in floor_waiting[f]:
                            if passengers[p].start == e.floor and len(e.passenger) < elev_max:
                                e.passenger[p] = passengers[p]
                                e.insert(f)
                                will_del.append(p)
                        for w in will_del:
                            floor_waiting[f].pop(w)
                        if len(e.passenger) >= elev_max:
                            break
                    # 타고 있는 승객을 빨리 내려주기 위해 이동 (각각의 엘리베이터가 일정 손님을 맡았으므로)
                    up_count = 0
                    down_count = 0
                    d_max = e.floor
                    d_min = e.floor
                    for d in e.destination:
                        if e.floor < d:
                            up_count += 1
                        elif e.floor > d:
                            down_count += 1
                        if d_max < d:
                            d_max = d
                        if d_min > d:
                            d_min = d
                    if d_max != e.final_destination and (d_min == e.final_destination or abs(d_max - e.floor) < abs(d_min - e.floor)):
                        e.set_final(d_max)
                    else:
                        e.set_final(d_min)
            # 이동하는 중 빈자리 있을 경우 이동 방향과 가는 방향이 같고 이동하는 쪽에 있는 가장 가까운 승객
            elif e.status == 'UPWARD' or e.direction == 1:
                for f in range(e.floor, n_floor):
                    will_del = []
                    for p in floor_waiting[f]:
                        if passengers[p].dir == 1 and len(e.passenger) < elev_max:
                            e.passenger[p] = passengers[p]
                            e.insert(f)
                            will_del.append(p)
                        elif passengers[p].start == e.final_destination and len(e.passenger) < elev_max:
                            e.passenger[p] = passengers[p]
                            e.insert(f)
                            will_del.append(p)
                    for w in will_del:
                        floor_waiting[f].pop(w)
                    if len(e.passenger) >= elev_max:
                        break
            # UPWARD 의 반대
            elif e.status == 'DOWNWARD' or e.direction == -1:
                for f in range(e.floor, 0, -1):
                    will_del = []
                    for p in floor_waiting[f]:
                        if passengers[p].dir == -1 and len(e.passenger) < elev_max:
                            e.passenger[p] = passengers[p]
                            e.insert(f)
                            will_del.append(p)
                        elif passengers[p].start == e.final_destination and len(e.passenger) < elev_max:
                            e.passenger[p] = passengers[p]
                            e.insert(f)
                            will_del.append(p)
                    for w in will_del:
                        floor_waiting[f].pop(w)
                    if len(e.passenger) >= elev_max:
                        break

        action_list = []
        # Add action
        for e in elevators:
            e = elevators[e]
            to_enter = e.passenger_check(floor_exist[e.floor])  # 현재 층에서 타야할 승객
            to_exit = e.need_to_exit(e.floor)  # 현재 층에서 내려야할 승객
            if e.status == 'STOPPED':
                if len(e.passenger) == 0:  # 승객 없을 경우
                    if len(to_enter) > 0:  # 태워야할 승객 있는 경우
                        action_list.append({'elevator_id': e.id, 'command': 'OPEN'})
                    elif e.final_destination > e.floor:  # 승객 없는데 위로 올라가야할 경우
                        action_list.append({'elevator_id': e.id, 'command': 'UP'})
                    elif e.final_destination < e.floor:  # 승객 없는데 아래로 내려가야할 경우
                        action_list.append({'elevator_id': e.id, 'command': 'DOWN'})
                    else:  # 승객 없는데 목적지 층일 경우
                        action_list.append({'elevator_id': e.id, 'command': 'OPEN'})
                elif len(to_exit) > 0 or len(to_enter) > 0:  # 승객 있는데 내릴 승객 혹은 탈 승객 있는 경우
                    action_list.append({'elevator_id': e.id, 'command': 'OPEN'})
                elif e.need_to_stop(e.floor):  # 멈춰야하는 경우
                    action_list.append({'elevator_id': e.id, 'command': 'OPEN'})
                elif e.final_destination > e.floor:  # 내리거나 탈 승객 없을 때 위로 올라가야할 경우
                    action_list.append({'elevator_id': e.id, 'command': 'UP'})
                elif e.final_destination < e.floor:  # 내리거나 탈 승객 없을 때 아래로 내려가야할 경우
                    action_list.append({'elevator_id': e.id, 'command': 'DOWN'})

            elif e.status == 'OPENED':
                if len(e.passenger) == 0:  # 승객 없을 경우
                    if e.final_destination == e.floor:  # 목적지 층일 경우
                        if len(to_enter) > 0:  # 태워야할 승객이 있을 경우
                            action_list.append({'elevator_id': e.id, 'command': 'ENTER', 'call_ids': to_enter})
                            e.enter_passenger(to_enter)
                            for p in to_enter:
                                floor_exist[e.floor].pop(p)
                        else:  # 목적지 층인데 태워야할 승객이 없을 경우
                            action_list.append({'elevator_id': e.id, 'command': 'OPEN'})
                            if e.floor in e.destination:
                                e.erase_destination(e.floor)
                    elif len(to_enter) > 0:  # 중간 층이지만 태워야할 승객 있는 경우
                        action_list.append({'elevator_id': e.id, 'command': 'ENTER', 'call_ids': to_enter})
                        e.enter_passenger(to_enter)
                        for p in to_enter:
                            floor_exist[e.floor].pop(p)
                    else:  # 승객 없는데 이동해야할 경우
                        action_list.append({'elevator_id': e.id, 'command': 'CLOSE'})

                elif e.need_to_stop(e.floor):  # 멈춰서야할 경우
                    if len(to_exit) == 0 and len(to_enter) == 0:  # 내리거나 탈 승객 없는 경우
                        action_list.append({'elevator_id': e.id, 'command': 'CLOSE'})
                        e.erase_destination(e.floor)
                    elif len(to_exit) > 0:  # 내릴 승객 있는 경우
                        action_list.append({'elevator_id': e.id, 'command': 'EXIT', 'call_ids': to_exit})
                        e.erase_passengers(to_exit)
                        for p in to_exit:
                            passengers.pop(p)
                    elif len(to_enter) > 0:  # 탈 승객 있는 경우
                        action_list.append({'elevator_id': e.id, 'command': 'ENTER', 'call_ids': to_enter})
                        e.enter_passenger(to_enter)
                        for p in to_enter:
                            floor_exist[e.floor].pop(p)
                    else:
                        action_list.append({'elevator_id': e.id, 'command': 'STOP'})
                elif e.final_destination != e.floor:
                    action_list.append({'elevator_id': e.id, 'command': 'CLOSE'})
                else:
                    action_list.append({'elevator_id': e.id, 'command': 'STOP'})

            elif e.status == 'UPWARD':
                # 올라가는 도중 현재 층에 내리거나 탈 승객이 있거나, 최종 목적지거나, 멈춰야하면 정지
                if len(to_enter) > 0 or len(to_exit) > 0 or e.floor == e.final_destination or e.need_to_stop(e.floor):
                    action_list.append({'elevator_id': e.id, 'command': 'STOP'})
                else:  # 아니면 올라간다
                    action_list.append({'elevator_id': e.id, 'command': 'UP'})

            elif e.status == 'DOWNWARD':
                # 내려가는 도중 현재 층에 내리거나 탈 승객이 있거나, 최종 목적지거나, 멈춰야하면 정지
                if len(to_enter) > 0 or len(to_exit) > 0 or e.floor == e.final_destination or e.need_to_stop(e.floor):
                    action_list.append({'elevator_id': e.id, 'command': 'STOP'})
                else:  # 아니면 내려간다
                    action_list.append({'elevator_id': e.id, 'command': 'DOWN'})

        print(action_list, '\n')
        action(token, action_list)


if __name__ == '__main__':
    p0_solution()
