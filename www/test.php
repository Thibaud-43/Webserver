 <html>
 <head>
  <title>Test PHP</title>
 </head>
 <body>
<?php
echo 'GET: Bonjour' . htmlspecialchars($_GET["name"]) . '!';
?>

<?php
echo 'POST: Bonjour ' . htmlspecialchars($_POST["name"]) . '!';
?>

 </body>
</html>
