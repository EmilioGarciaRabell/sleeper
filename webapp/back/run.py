from pathlib import Path
import uuid
import wave

from flask_sock import Sock
from redis import Redis
from rq import Queue

from app import create_app

flask_app = create_app()
sock = Sock(flask_app)

redis_conn = Redis(host="redis", port=6379, db=0)
task_queue = Queue(connection=redis_conn)


@sock.route('/receive_data')
def websocket_route(ws):
    channels = 1
    sample_width = 2
    frame_rate = 16000

    output_dir = Path(__file__).resolve().parent.parent / "back" / "app" / "storage" / "audio_files"
    output_dir.mkdir(parents=True, exist_ok=True)
    output_file_path = output_dir / f"{uuid.uuid4()}.wav"

    total_bytes = 0

    try:
        with wave.open(str(output_file_path), "wb") as output_file:
            output_file.setnchannels(channels)
            output_file.setsampwidth(sample_width)
            output_file.setframerate(frame_rate)

            while True:
                try:
                    data = ws.receive()
                except Exception:
                    break

                if data is None or isinstance(data, str) or len(data) == 0:
                    break

                output_file.writeframes(data)
                total_bytes += len(data)
    except Exception:
        pass

    if total_bytes <= 0:
        try:
            output_file_path.unlink()
        except FileNotFoundError:
            pass
        return

    task_queue.enqueue('app.api.background_tasks.save_file', str(output_file_path))


if __name__ == '__main__':
    sock.init_app(flask_app)
    flask_app.run(host="0.0.0.0", port=5000, debug=False, use_reloader=False)