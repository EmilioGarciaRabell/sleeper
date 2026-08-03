
CREATE TABLE notes(
    id INT AUTO_INCREMENT PRIMARY KEY 
    date_posted DATE DEFAULT NOW(),
    audio_path VARCHAR(4000),
    text_path VARCHAR(4000)
)

