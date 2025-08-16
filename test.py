from dataclasses import dataclass
import time
import os


@dataclass
class User:
    id: int
    name: str
    age: int


class UserSoA:
    def __init__(self):
        self.ids = []
        self.names = []
        self.ages = []

    @classmethod
    def new_with_capacity(cls, capacity: int) -> "UserSoA":
        return cls()

    def add_user(self, id: int, name: str, age: int) -> None:
        self.ids.append(id)
        self.names.append(name)
        self.ages.append(age)


def main() -> None:
    num_users = int(os.getenv("NUM_USERS", "100000000"))

    users: list[User] = []
    user_soa: UserSoA = UserSoA.new_with_capacity(num_users)

    for i in range(num_users):
        uid = int(i)
        name = f"User {i}"
        age = i % 100
        users.append(User(id=uid, name=name, age=age))
        user_soa.add_user(uid, name, age)

    t0 = time.perf_counter()
    total_age = sum(u.age for u in users)
    avg_age = total_age / len(users)
    elapsed = time.perf_counter() - t0

    print(f"Average age: {avg_age:.0f}")
    print(f"Elapsed time: {elapsed:.6f}s")

    t0 = time.perf_counter()
    total_age_soa = sum(user_soa.ages)
    avg_age_soa = total_age_soa / len(user_soa.ages)
    elapsed_soa = time.perf_counter() - t0

    print(f"Average age soa: {avg_age_soa:.0f}")
    print(f"Elapsed time soa: {elapsed_soa:.6f}s")


if __name__ == "__main__":
    main()