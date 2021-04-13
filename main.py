import socket
import tkinter
import threading

maneuver: int = 4
root = tkinter.Tk()
canvas = tkinter.Canvas(root, height=400, width=400, bg='white')
canvas.pack()
maneuver_img = tkinter.PhotoImage(file='src/autopilot.png')


def run_server():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    print("Socket created")
    s.bind((socket.gethostname(), 1228))
    s.listen(5)
    global maneuver
    global canvas
    i = 1
    while True:
        client_socket, address = s.accept()
        while True:
            content = client_socket.recv(32)
            content = str(content, 'utf-8')
            if len(content) == 0:
                break
            else:
                if content == '0' and maneuver != 0:
                    print(f'{i} LANE_CHANGE_LEFT')
                    maneuver = 0
                    maneuver_img.configure(file='src/links.png')
                elif content == '1' and maneuver != 1:
                    print(f'{i} LANE_CHANGE_RIGHT')
                    maneuver = 1
                    maneuver_img.configure(file='src/rechts.png')
                elif content == '2' and maneuver != 2:
                    print(f'{i} FASTER_10')
                    maneuver = 2
                    maneuver_img.configure(file='src/faster.png')
                elif content == '3' and maneuver != 3:
                    print(f'{i} SLOWER_10')
                    maneuver = 3
                    maneuver_img.configure(file='src/slower.png')
                elif content == '4':
                    maneuver_img.configure(file='src/autopilot.png')
                i += 1
        client_socket.close()


def main():
    global maneuver
    thread = threading.Thread(target=run_server)
    thread.deamon = True
    thread.start()

    global canvas
    global root
    global maneuver_img

    root.geometry("400x400")
    root.minsize(width="200", height="200")
    root.title("Cube Demo")

    canvas.create_image(200, 200, image=maneuver_img)

    root.mainloop()

if __name__ == '__main__':
    main()