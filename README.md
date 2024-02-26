# Synchronizing Sages: Mastering Concurrency with Philosophers

Embark on a journey into the realm of concurrent programming with the enthralling project titled "Philosophers." This simulation tackles the timeless dilemma of dining philosophers using threads and mutexes, offering invaluable insights into concurrency, synchronization, and resource management.

### Key Components:

1. **Threads and Processes:** Philosophers are represented by threads in the mandatory part and processes in the bonus section. Threads and processes enable concurrent execution of philosophers' actions, mirroring their simultaneous behavior around the table.
2. **Mutexes:** Essential for synchronizing access to shared resources like forks. Each fork is protected by a mutex to prevent concurrent access conflicts.
3. **Resource Management:** In the mandatory part, each philosopher has a fork on their left and right. Mutexes manage fork access to prevent concurrency issues. In the bonus part, the number of forks is represented by a semaphore, ensuring orderly access to available forks.
4. **Logs and Messages:** Implementation of detailed logs describing philosophers' actions such as taking a fork, eating, sleeping, thinking, and dying. These messages are formatted with appropriate timestamps and must not mix with other messages.
5. **State Management:** Crucial for handling philosophers' states to avoid concurrency problems and ensure consistent simulation operation. Mutexes ensure each philosopher transitions through states in an orderly, conflict-free manner.

This project offers participants a hands-on opportunity to grasp and apply concurrent programming concepts, thread/process synchronization, and shared resource management. By tackling this challenge, developers gain valuable experience in solving complex concurrency and synchronization issues in multithreaded applications.
